// Retropsis @ 2024

#include "Equipment/Tool.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Player/PlayerCharacter.h"
#include "Player/PlayerCharacterController.h"

ATool::ATool()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void ATool::InitializeTool()
{
	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetSphereRadius(125.f);
	AreaSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AreaSphere->SetupAttachment(RootComponent);

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidget->SetupAttachment(RootComponent);
	PickupWidget->SetWidgetSpace(EWidgetSpace::Screen);
	PickupWidget->SetDrawAtDesiredSize(true);
	PickupWidget->SetRelativeLocation(FVector(0.f, 0.f, 75.f));
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
	// TODO: Is Overriden in Weapon
	PlayActiveAnimation();
}

void ATool::Drop()
{
	SetToolState(EToolState::ETS_Dropped);
	DetachToolFromComponent(); // TODO: Is Overriden in Weapon
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
		OwnerCharacter = OwnerCharacter == nullptr ? Cast<APlayerCharacter>(Owner) : OwnerCharacter;
		if(OwnerCharacter && OwnerCharacter->GetEquippedTool() == this)
		{
			SetHUDAmmunition();
		}
		// TODO: Could add ammo HUD for Secondary if needed
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
	OnToolStateSet();
}

void ATool::OnRep_ToolState()
{
	OnToolStateSet();
}

void ATool::OnToolStateSet()
{
	switch (ToolState)
	{
	case EToolState::ETS_Initial:
		break;
	case EToolState::ETS_Equipped:
		OnEquipped();
		break;
	case EToolState::ETS_Dropped:
		OnDropped();
		break;
	case EToolState::ETS_Secondary:
		OnSecondary();
		break;
	default: ;
	}
}

void ATool::OnEquipped()
{
	ShowPickupWidget(false);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if(EquipSound) UGameplayStatics::PlaySoundAtLocation(this, EquipSound, GetActorLocation());
}

void ATool::OnDropped()
{
	if(HasAuthority())
	{
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		AreaSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	}
	if(DropSound) UGameplayStatics::PlaySoundAtLocation(this, DropSound, GetActorLocation());
}

void ATool::OnSecondary()
{
	ShowPickupWidget(false);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if(EquipSound) UGameplayStatics::PlaySoundAtLocation(this, EquipSound, GetActorLocation());
}

TArray<FGameplayTag> ATool::GetToolAbilityTags() const
{
	TArray<FGameplayTag> AbilityTags;
	for (TTuple<FGameplayTag, FGameplayTag> Ability : ToolAbilities)
	{
		AbilityTags.Add(Ability.Key);
	}
	return AbilityTags;
}

FVector ATool::GetCombatSocket() const
{
	if(GetMesh())
	{
		return GetMesh()->GetSocketLocation(CombatSocket);
	}
	return FVector();
}
