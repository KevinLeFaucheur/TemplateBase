// Retropsis @ 2024

#include "Equipment/Tool.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Equipment/EquipmentComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Player/PlayerCharacter.h"
#include "Player/PlayerCharacterController.h"

ATool::ATool()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionResponseToAllChannels(ECR_Block);
	Mesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(Mesh);

	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetupAttachment(RootComponent);
	AreaSphere->SetSphereRadius(150.f);
	AreaSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidget->SetupAttachment(RootComponent);
	PickupWidget->SetWidgetSpace(EWidgetSpace::Screen);
	PickupWidget->SetDrawAtDesiredSize(true);
}

void ATool::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void ATool::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATool, ToolState);
}

void ATool::BeginPlay()
{
	Super::BeginPlay();
	SetReplicateMovement(true);
	
	if (HasAuthority())
	{
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		AreaSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &ATool::OnSphereBeginOverlap);
		AreaSphere->OnComponentEndOverlap.AddDynamic(this, &ATool::OnSphereEndOverlap);
	}
	ShowPickupWidget(false);
}

void ATool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/*
 * Activation
*/
void ATool::Activate()
{
}

void ATool::Activate(const FVector& HitTarget)
{
	PlayFireAnimation();
}

void ATool::PlayFireAnimation()
{
	if(ActiveAnimation) Mesh->PlayAnimation(ActiveAnimation, false);
}

void ATool::Drop()
{
	SetToolState(EToolState::ETS_Dropped);
	const FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
	Mesh->DetachFromComponent(DetachRules);
	SetOwner(nullptr); 
	OwnerCharacter = nullptr;
	OwnerController = nullptr;
}

void ATool::OnRep_Owner()
{
	Super::OnRep_Owner();
	if(Owner == nullptr)
	{
		OwnerCharacter = nullptr;
		OwnerController = nullptr;
	}
	else
	{
		SetHUDAmmunition();
	}
}

/*
 * Equipment Interface
 */
FEquipmentInfo ATool::GetEquipmentInfo_Implementation()
{
	return FEquipmentInfo{ MainHandSocket, AnimationState };
}

void ATool::UseItem_Implementation(AActor* PlayerCharacter)
{
	
}

void ATool::MontageEnd_Implementation() {}	

/*
 * Interaction
 */
void ATool::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(APlayerCharacter* PC = Cast<APlayerCharacter>(OtherActor))
	{
		PC->SetOverlappingTool(this);
	}
}

void ATool::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(APlayerCharacter* PC = Cast<APlayerCharacter>(OtherActor))
	{
		PC->SetOverlappingTool(nullptr);
	}
}

void ATool::ShowPickupWidget(bool bShowWidget)
{
	if(PickupWidget) PickupWidget->SetVisibility(bShowWidget);
}

/*
 * State
 */
void ATool::SetToolState(const EToolState NewState)
{
	ToolState = NewState;
	
	switch (ToolState) {
	case EToolState::ETS_Initial:
		break;
	case EToolState::ETS_Equipped:
		ShowPickupWidget(false);
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Mesh->SetSimulatePhysics(false);
		Mesh->SetEnableGravity(false);
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if(EquipSound) UGameplayStatics::PlaySoundAtLocation(this, EquipSound, GetActorLocation());
		if(bUsePhysicsAsset)
		{
			Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			Mesh->SetEnableGravity(true);
			Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
		}
		break;
	case EToolState::ETS_Dropped:
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		AreaSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		Mesh->SetSimulatePhysics(true);
		Mesh->SetEnableGravity(true);
		Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Mesh->SetCollisionResponseToAllChannels(ECR_Block);
		Mesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		Mesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
		if(DropSound) UGameplayStatics::PlaySoundAtLocation(this, DropSound, GetActorLocation());
		break;
	default: ;
	}
}

void ATool::OnRep_ToolState()
{
	switch (ToolState) {
	case EToolState::ETS_Initial:
		break;
	case EToolState::ETS_Equipped:
		ShowPickupWidget(false);
		Mesh->SetSimulatePhysics(false);
		Mesh->SetEnableGravity(false);
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if(EquipSound) UGameplayStatics::PlaySoundAtLocation(this, EquipSound, GetActorLocation());
		if(bUsePhysicsAsset)
		{
			Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			Mesh->SetEnableGravity(true);
			Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
		}
		break;
	case EToolState::ETS_Dropped:
		Mesh->SetSimulatePhysics(true);
		Mesh->SetEnableGravity(true);
		Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Mesh->SetCollisionResponseToAllChannels(ECR_Block);
		Mesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		Mesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
		if(DropSound) UGameplayStatics::PlaySoundAtLocation(this, DropSound, GetActorLocation());
		break;
	default: ;
	}
}
