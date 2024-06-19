// Retropsis @ 2024

#include "Equipment/Tool.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Equipment/Weapon/Casing.h"
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

void ATool::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATool, ToolState);
	DOREPLIFETIME(ATool, Ammunition);
}

void ATool::BeginPlay()
{
	Super::BeginPlay();
	
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
void ATool::Activate(const FVector& HitTarget)
{
	if(ActiveAnimation) Mesh->PlayAnimation(ActiveAnimation, false);
	if(CasingClass)
	{
		if(const USkeletalMeshSocket* AmmoEjectSocket = Mesh->GetSocketByName(FName("AmmoEject")))
		{
			const FTransform SocketTransform = AmmoEjectSocket->GetSocketTransform(Mesh);
			if(UWorld* World = GetWorld())
			{
				World->SpawnActor<ACasing>(CasingClass, SocketTransform.GetLocation(), SocketTransform.GetRotation().Rotator());
			}
		}
	}
	SpendAmmunition();
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

void ATool::AddAmmunition(int32 AmountToAdd)
{
	Ammunition = FMath::Clamp(Ammunition + AmountToAdd, 0, AmmunitionCapacity);
	SetHUDAmmunition();
}

void ATool::SpendAmmunition()
{
	Ammunition = FMath::Clamp(Ammunition - 1, 0, AmmunitionCapacity);
	SetHUDAmmunition();
}

void ATool::OnRep_Ammunition()
{
	SetHUDAmmunition();
}

void ATool::SetHUDAmmunition()
{
	OwnerCharacter = OwnerCharacter == nullptr ? OwnerCharacter = Cast<APlayerCharacter>(GetOwner()) : OwnerCharacter;
	if(OwnerCharacter)
	{
		OwnerController = OwnerController == nullptr ? Cast<APlayerCharacterController>(OwnerCharacter->Controller) : OwnerController;
		if(OwnerController)
		{
			OwnerController->SetHUDAmmunition(Ammunition);
		}
	}
}

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
		// if(bUsePhysicsAsset)
		// {
		// 	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		// 	Mesh->SetEnableGravity(true);
		// 	Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
		// }
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

bool ATool::IsEmpty()
{
	return Ammunition <= 0;
}