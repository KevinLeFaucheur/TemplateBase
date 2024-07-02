// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/PlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "BaseGameplayTags.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "Equipment/EquipmentComponent.h"
#include "Equipment/Tool.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Player/CharacterAnimInstance.h"
#include "Player/PlayerCharacterController.h"
#include "Player/PlayerCharacterState.h"
#include "NiagaraComponent.h"
#include "AbilitySystem/StatusEffect/StatusEffectNiagaraComponent.h"
#include "Data/ToolInfo.h"
#include "Inventory/HotbarComponent.h"
#include "Inventory/PlayerInventoryComponent.h"

APlayerCharacter::APlayerCharacter()
{
	NetUpdateFrequency = 66.f;
	MinNetUpdateFrequency = 33.f;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(RootComponent);

	EquipmentComponent = CreateDefaultSubobject<UEquipmentComponent>(TEXT("EquipmentComponent"));
	EquipmentComponent->SetIsReplicated(true);

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	TurningInPlace = ETurningInPlace::ETIP_NotTurning;

	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("LevelUpNiagaraComponent");
	LevelUpNiagaraComponent->SetupAttachment(GetRootComponent());
	LevelUpNiagaraComponent->bAutoActivate = false;

	AttachedThrowable = CreateDefaultSubobject<UStaticMeshComponent>("AttachedThrowable");
	AttachedThrowable->SetupAttachment(GetMesh(), FName("ThrowableSocket"));
	AttachedThrowable->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PlayerInventory = CreateDefaultSubobject<UPlayerInventoryComponent>("PlayerInventory");
	HotbarComponent = CreateDefaultSubobject<UHotbarComponent>("Hotbar");
	// PlayerInventory->SetInventorySize(30);
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(APlayerCharacter, OverlappingTool, COND_OwnerOnly);
	DOREPLIFETIME(APlayerCharacter, bIsCasting);
}

void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if(EquipmentComponent) EquipmentComponent->PlayerCharacter = this;
}

/*
 * GAS
 */
void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init Actor Info - Server
	InitAbilityActorInfo();
	AddCharacterAbilities();
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	if(AttachedThrowable) AttachedThrowable->SetVisibility(false);
}

void APlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	// Init Actor Info - Client
	InitAbilityActorInfo();
}

void APlayerCharacter::InitAbilityActorInfo()
{
	APlayerCharacterState* PlayerCharacterState = GetPlayerState<APlayerCharacterState>();
	check(PlayerCharacterState);
	PlayerCharacterState->GetAbilitySystemComponent()->InitAbilityActorInfo(PlayerCharacterState, this);
	Cast<UBaseAbilitySystemComponent>(PlayerCharacterState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = PlayerCharacterState->GetAbilitySystemComponent();
	AttributeSet = PlayerCharacterState->GetAttributeSet();
	OnASCRegistered.Broadcast(AbilitySystemComponent);
	AbilitySystemComponent->RegisterGameplayTagEvent(FBaseGameplayTags::Get().StatusEffect_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &APlayerCharacter::StunTagChanged);

	if (APlayerCharacterController* PlayerCharacterController = Cast<APlayerCharacterController>(GetController()))
	{
		if (APlayerHUD* PlayerHUD = PlayerCharacterController->GetHUD<APlayerHUD>())
		{
			PlayerHUD->InitOverlay(PlayerCharacterController, PlayerCharacterState, AbilitySystemComponent, AttributeSet);
		}
	}
	InitializeDefaultAttributes();
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(GetLocalRole() > ROLE_SimulatedProxy && IsLocallyControlled())
	{
		AimOffset(DeltaTime);
	}
	else
	{
		TimeSinceLastMovementReplication += DeltaTime;
		if(TimeSinceLastMovementReplication > 0.25f) OnRep_ReplicatedMovement();
		CalculateAO_Pitch();
	}
	HideCharacterIfCameraClose();
}

/*
 * Movement
 */
void APlayerCharacter::Jump()
{
	if(bIsCrouched) UnCrouch();
	else Super::Jump();
}

void APlayerCharacter::OnRep_ReplicatedMovement()
{
	Super::OnRep_ReplicatedMovement();
	SimProxiesTurn();
	TimeSinceLastMovementReplication = 0.f;
}

float APlayerCharacter::CalculateSpeed()
{
	return UKismetMathLibrary::VSizeXY(GetVelocity());
}

/*
 * Aiming
 */
void APlayerCharacter::AimOffset(float DeltaTime)
{
	if(EquipmentComponent && EquipmentComponent->EquippedTool == nullptr) return;
	if(!EquipmentComponent->EquippedTool->bUseAimOffsets) return;
	const float Speed= CalculateSpeed();
	const bool bAirborne = GetCharacterMovement()->IsFalling();
	
	if(Speed == 0.f && !bAirborne)
	{
		bRotateRootBone = true;
		const FRotator CurrentAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		const FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrentAimRotation, StartingAimRotation);
		AO_Yaw = DeltaAimRotation.Yaw;
		if(TurningInPlace == ETurningInPlace::ETIP_NotTurning)
		{
			InterpAO_Yaw = AO_Yaw;
		}
		bUseControllerRotationYaw = true;
		TurnInPlace(DeltaTime);
	}
	if(Speed > 0.f || bAirborne)
	{
		bRotateRootBone = false;
		StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		AO_Yaw = 0.f;
		bUseControllerRotationYaw = true;
		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
	}
	CalculateAO_Pitch();
}

void APlayerCharacter::CalculateAO_Pitch()
{
	AO_Pitch = GetBaseAimRotation().Pitch;
	if(AO_Pitch > 90.f && !IsLocallyControlled())
	{
		// Map pitch from [270, 360) to [-90, 0)
		const FVector2D InRange(270.f, 360.f);
		const FVector2D OutRange(-90.f, 0.f);
		AO_Pitch = FMath::GetMappedRangeValueClamped(InRange, OutRange, AO_Pitch);
	}
}

void APlayerCharacter::TurnInPlace(float DeltaTime)
{
	if(AO_Yaw > 90.f)
	{
		TurningInPlace = ETurningInPlace::ETIP_Right;
	}
	else if (AO_Yaw < -90.f)
	{
		TurningInPlace = ETurningInPlace::ETIP_Left;
	}
	if(TurningInPlace != ETurningInPlace::ETIP_NotTurning)
	{
		InterpAO_Yaw = FMath::FInterpTo(InterpAO_Yaw, 0.f, DeltaTime, 4.f);
		AO_Yaw = InterpAO_Yaw;
		if(FMath::Abs(AO_Yaw) < AngleBeforeTurning)
		{
			TurningInPlace = ETurningInPlace::ETIP_NotTurning;
			StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		}
	}
	
}

void APlayerCharacter::SimProxiesTurn()
{
	if(EquipmentComponent && EquipmentComponent->EquippedTool == nullptr) return;
	
	bRotateRootBone = false;
	
	if(const float Speed= CalculateSpeed(); Speed > 0.f)
	{
		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
		return;
	}
	// CalculateAO_Pitch();

	ProxyRotationLastFrame = ProxyRotation;
	ProxyRotation = GetActorRotation();
	ProxyYaw = UKismetMathLibrary::NormalizedDeltaRotator(ProxyRotation, ProxyRotationLastFrame).Yaw;

	if(FMath::Abs(ProxyYaw) > TurnThreshold)
	{
		if(ProxyYaw > TurnThreshold) TurningInPlace = ETurningInPlace::ETIP_Right;
		else if (ProxyYaw< -TurnThreshold) TurningInPlace = ETurningInPlace::ETIP_Left;
		else TurningInPlace = ETurningInPlace::ETIP_NotTurning;
		return;
	}
	TurningInPlace = ETurningInPlace::ETIP_NotTurning;
}

/*
 * Equipping
 */
void APlayerCharacter::EquipButtonPressed()
{
	if(EquipmentComponent)
	{
		if(HasAuthority())
		{
			EquipmentComponent->EquipTool(OverlappingTool);
		}
		else
		{
			ServerEquipButtonPressed();
		}
	}
}

void APlayerCharacter::ServerEquipButtonPressed_Implementation()
{
	if(EquipmentComponent)
	{
		EquipmentComponent->EquipTool(OverlappingTool);
	}
}

void APlayerCharacter::SetOverlappingTool(ATool* Tool)
{
	if(IsLocallyControlled()) if(OverlappingTool) OverlappingTool->ShowPickupWidget(false);
	OverlappingTool = Tool;
	if(IsLocallyControlled()) if(OverlappingTool) OverlappingTool->ShowPickupWidget(true);
}

void APlayerCharacter::OnRep_OverlappingTool(ATool* LastTool)
{
	if(OverlappingTool) OverlappingTool->ShowPickupWidget(true);
	if(LastTool) LastTool->ShowPickupWidget(false);
}

/*
 * Firing
*/
void APlayerCharacter::ServerLeftMouseButtonPressed_Implementation()
{
	if(EquippedTool) IEquipmentInterface::Execute_UseItem(EquippedTool, this);
}

void APlayerCharacter::FireButtonPressed()
{
	// TODO: Need to streamline some other way
	ServerLeftMouseButtonPressed();
	if(EquipmentComponent) EquipmentComponent->FireButtonPressed(true);
}

void APlayerCharacter::FireButtonReleased()
{
	if(EquipmentComponent) EquipmentComponent->FireButtonPressed(false);
}

void APlayerCharacter::PlayFireMontage(bool bAiming)
{
	if(!IsEquipped()) return;
	if(UCharacterAnimInstance* AnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance()))
	{
		AnimInstance->PlayFireMontage(bAiming);
	}
}

void APlayerCharacter::PlayThrowMontage()
{
	if(UCharacterAnimInstance* AnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance()))
	{
		AnimInstance->PlayThrowMontage();
	}
}

void APlayerCharacter::PlayHitReactMontage()
{
	MulticastHitReact();
}

void APlayerCharacter::MulticastHitReact_Implementation()
{
	if(!IsEquipped()) return;
	if(UCharacterAnimInstance* AnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance()))
	{
		AnimInstance->PlayHitReactMontage();
	}
}

/*
 * Aiming
 */
void APlayerCharacter::AimButtonPressed()
{
	if(EquipmentComponent) EquipmentComponent->SetAiming(true);
}

void APlayerCharacter::AimButtonReleased()
{
	if(EquipmentComponent) EquipmentComponent->SetAiming(false);
}

/*
 * Crouching
 */
void APlayerCharacter::CrouchButtonPressed()
{
	bIsCrouched ? UnCrouch() : Crouch();
}

/*
 * Reloading
 */
void APlayerCharacter::ReloadButtonPressed()
{
	if(EquipmentComponent) EquipmentComponent->Reload();
}

void APlayerCharacter::ReloadEnd()
{
	if(EquipmentComponent) EquipmentComponent->ReloadEnd();
}

void APlayerCharacter::PlayReloadMontage(const EToolType ToolType)
{
	if(UCharacterAnimInstance* AnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance()))
	{
		AnimInstance->PlayReloadMontage(ToolType);
	}
}

/*
 * Throwing
 */
void APlayerCharacter::ThrowButtonPressed()
{
	if(EquipmentComponent) EquipmentComponent->Throw();
}

/*
 * QoL
 */
void APlayerCharacter::HideCharacterIfCameraClose()
{
	if(!IsLocallyControlled()) return;

	const bool bCameraTooClose = (FollowCamera->GetComponentLocation() - GetActorLocation()).Size() < CameraThreshold;
	GetMesh()->SetVisibility(!bCameraTooClose);
	if(EquipmentComponent && EquipmentComponent->EquippedTool && EquipmentComponent->EquippedTool->GetMesh())
	{
		EquipmentComponent->EquippedTool->GetMesh()->bOwnerNoSee = bCameraTooClose;
	}
}

/*
 * Combat Interface
 */
int32 APlayerCharacter::GetCharacterLevel_Implementation()
{
	const APlayerCharacterState* PlayerCharacterState = GetPlayerState<APlayerCharacterState>();
	check(PlayerCharacterState);
	return PlayerCharacterState->GetPlayerLevel();
}

int32 APlayerCharacter::GetXP_Implementation() const
{
	const APlayerCharacterState* PlayerCharacterState = GetPlayerState<APlayerCharacterState>();
	check(PlayerCharacterState);
	return PlayerCharacterState->GetXP();
}

int32 APlayerCharacter::GetAttributePointReward_Implementation(int32 Level) const
{
	const APlayerCharacterState* PlayerCharacterState = GetPlayerState<APlayerCharacterState>();
	check(PlayerCharacterState);
	return PlayerCharacterState->LevelUpInfo->LevelUpInformation[Level].AttributePointAward;
}

int32 APlayerCharacter::GetSpellPointReward_Implementation(int32 Level) const
{
	const APlayerCharacterState* PlayerCharacterState = GetPlayerState<APlayerCharacterState>();
	check(PlayerCharacterState);
	return PlayerCharacterState->LevelUpInfo->LevelUpInformation[Level].SpellPointAward;
}

int32 APlayerCharacter::GetAttributePoints_Implementation() const
{
	const APlayerCharacterState* PlayerCharacterState = GetPlayerState<APlayerCharacterState>();
	check(PlayerCharacterState);
	return PlayerCharacterState->GetAttributePoints();
}

int32 APlayerCharacter::GetSpellPoints_Implementation() const
{
	const APlayerCharacterState* PlayerCharacterState = GetPlayerState<APlayerCharacterState>();
	check(PlayerCharacterState);
	return PlayerCharacterState->GetSpellPoints();
}

void APlayerCharacter::AddToPlayerLevel_Implementation(int32 InPlayerLevel)
{
	APlayerCharacterState* PlayerCharacterState = GetPlayerState<APlayerCharacterState>();
	check(PlayerCharacterState);
	PlayerCharacterState->AddToLevel(InPlayerLevel);

	if (UBaseAbilitySystemComponent* BaseASC = Cast<UBaseAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		BaseASC->UpdateAbilityStatuses(PlayerCharacterState->GetPlayerLevel());
	}
}

void APlayerCharacter::AddToAttributePoints_Implementation(int32 InAttributePoints)
{
	APlayerCharacterState* PlayerCharacterState = GetPlayerState<APlayerCharacterState>();
	check(PlayerCharacterState);
	PlayerCharacterState->AddToAttributePoints(InAttributePoints);
}

void APlayerCharacter::AddToSpellPoints_Implementation(int32 InSpellPoints)
{
	APlayerCharacterState* PlayerCharacterState = GetPlayerState<APlayerCharacterState>();
	check(PlayerCharacterState);
	PlayerCharacterState->AddToSpellPoints(InSpellPoints);
}

int32 APlayerCharacter::FindLevelForXP_Implementation(int32 InXP) const
{
	const APlayerCharacterState* PlayerCharacterState = GetPlayerState<APlayerCharacterState>();
	check(PlayerCharacterState);
	return PlayerCharacterState->LevelUpInfo->FindLevelForXP(InXP);
}

void APlayerCharacter::AddToXP_Implementation(int32 InXP)
{
	
	APlayerCharacterState* PlayerCharacterState = GetPlayerState<APlayerCharacterState>();
	check(PlayerCharacterState);
	PlayerCharacterState->AddToXP(InXP);
}

void APlayerCharacter::LevelUp_Implementation()
{
	MulticastLevelUp();
}

void APlayerCharacter::MulticastLevelUp_Implementation() const
{
	if(IsValid(LevelUpNiagaraComponent))
	{
		const FVector CameraLocation = FollowCamera->GetComponentLocation();
		const FVector NiagaraSystemLocation = LevelUpNiagaraComponent->GetComponentLocation();
		const FRotator ToCameraRotation =  (CameraLocation - NiagaraSystemLocation).Rotation();
		LevelUpNiagaraComponent->SetWorldRotation(ToCameraRotation);
		LevelUpNiagaraComponent->Activate(true);
	}
}

void APlayerCharacter::ToggleMagicCircle_Implementation(bool bShow, UMaterialInterface* DecalMaterial)
{
	if (APlayerCharacterController* PlayerCharacterController = Cast<APlayerCharacterController>(GetController()))
	{
		PlayerCharacterController->ToggleMagicCircle(bShow, DecalMaterial);
		// PlayerCharacterController->bShowMouseCursor = !bShow;
	}
}

void APlayerCharacter::PickupAmmunition_Implementation(EToolType ToolType, int32 AmmunitionAmount)
{
	EquipmentComponent->PickupAmmunition(ToolType, AmmunitionAmount);
}

void APlayerCharacter::UpdateInventorySlot_Implementation(EContainerType ContainerType, int32 SlotIndex, FInventoryItemData ItemData)
{
	IControllerInterface::Execute_UpdateInventorySlot(Controller, ContainerType, SlotIndex, ItemData);
}

void APlayerCharacter::OnSlotDrop_Implementation(EContainerType TargetContainer, EContainerType SourceContainer, int32 SourceSlotIndex, int32 TargetSlotIndex, EArmorType ArmorType)
{
	ServerOnSlotDrop(TargetContainer, SourceContainer, SourceSlotIndex, TargetSlotIndex, ArmorType);
}

void APlayerCharacter::ResetInventorySlot_Implementation(EContainerType ContainerType, int32 SlotIndex)
{
	IControllerInterface::Execute_ResetInventorySlot(Controller, ContainerType, SlotIndex);
}

void APlayerCharacter::PlayMontage_Implementation(UAnimMontage* Montage)
{
	MulticastPlayMontage(Montage);
}

void APlayerCharacter::MulticastPlayMontage_Implementation(UAnimMontage* Montage)
{
	bIsUsingItem = true;
	GetMesh()->GetAnimInstance()->Montage_Play(Montage);
	FOnMontageEnded MontageCompleted;
	MontageCompleted.BindWeakLambda(this, [this](UAnimMontage* AnimMontage, bool bInterrupted)
	{
		if(EquippedTool)
		{
			IEquipmentInterface::Execute_MontageEnd(EquippedTool);
		}
		bIsUsingItem = false;
	});
	GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(MontageCompleted, Montage);
}

/*
 * Status Effects
 */
void APlayerCharacter::OnRep_IsStunned()
{
	if(UBaseAbilitySystemComponent* BaseASC = Cast<UBaseAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		const FBaseGameplayTags GameplayTags = FBaseGameplayTags::Get();
		FGameplayTagContainer BlockedTags;
		BlockedTags.AddTag(GameplayTags.Player_Block_CursorTrace);
		BlockedTags.AddTag(GameplayTags.Player_Block_InputHeld);
		BlockedTags.AddTag(GameplayTags.Player_Block_InputPressed);
		BlockedTags.AddTag(GameplayTags.Player_Block_InputReleased);
		if(bIsStunned)
		{
			BaseASC->AddLooseGameplayTags(BlockedTags);
			StunStatusEffectComponent->Activate();
		}
		else
		{
			BaseASC->RemoveLooseGameplayTags(BlockedTags);
			StunStatusEffectComponent->Deactivate();
		}
	}
}

void APlayerCharacter::OnRep_IsBurning()
{
	if(bIsBurning) BurnStatusEffectComponent->Activate();
	else BurnStatusEffectComponent->Deactivate();
}

/*
 * Inventory
 */
void APlayerCharacter::ServerOnSlotDrop_Implementation(const EContainerType TargetContainer, const EContainerType SourceContainer, const int32 SourceSlotIndex, const int32 TargetSlotIndex, const EArmorType ArmorType)
{
	OnSlotDrop(TargetContainer, SourceContainer, SourceSlotIndex, TargetSlotIndex, ArmorType);
}

void APlayerCharacter::OnSlotDrop(const EContainerType TargetContainer, const EContainerType SourceContainer, const int32 SourceSlotIndex, const int32 TargetSlotIndex, EArmorType ArmorType) const
{
	UInventoryComponent* TargetInventory = nullptr;
	UInventoryComponent* SourceInventory = nullptr;
	
	switch (TargetContainer)
	{
	case EContainerType::Inventory:
		TargetInventory = PlayerInventory;
		break;
	case EContainerType::Hotbar:
		break;
	case EContainerType::Storage:
		break;
	case EContainerType::Armor:
		break;
	default: ;
	}
	
	switch (SourceContainer)
	{
	case EContainerType::Inventory:
		SourceInventory = PlayerInventory;
		break;
	case EContainerType::Hotbar:
		break;
	case EContainerType::Storage:
		break;
	case EContainerType::Armor:
		break;
	default: ;
	}
	if(TargetInventory && SourceInventory) TargetInventory->OnSlotDrop(SourceInventory, SourceSlotIndex, TargetSlotIndex);
}

void APlayerCharacter::ServerUseHotbarSlot_Implementation(int32 Index)
{
	if(!bIsUsingItem)
	{
		CurrentHotbarIndex = Index;
		EItemType ItemType;
		if(HotbarComponent->CheckHotbarAtIndex(Index, ItemType))
		{
			switch (ItemType) {
			case EItemType::Misc:
				break;
			case EItemType::Resource:
				break;
			case EItemType::Equipment:
				if(IsValid(EquippedTool))
				{
					EquippedTool->Destroy();
					MulticastUnequipItem();
				}
				else
				{
					const FInventoryItemData ItemData =  HotbarComponent->GetItemAtIndex(CurrentHotbarIndex);
					const UToolInfo* ToolInfo = Cast<UToolInfo>(ItemData.Asset.LoadSynchronous());
					UClass* Class = ToolInfo->ToolData.ToolClass.LoadSynchronous();
					ServerSpawnEquipment(Class, CurrentHotbarIndex);
				}
				break;
			case EItemType::Armor:
				break;
			case EItemType::Consumable:
				break;
			case EItemType::Buildable:
				break;
			default: ;
			}
		}
	}
}

void APlayerCharacter::ServerSpawnEquipment_Implementation(TSubclassOf<AActor> Class, int32 Index)
{
	EquippedTool = GetWorld()->SpawnActor<AActor>(Class);
	if(EquippedTool)
	{
		EquippedTool->SetOwner(this);
		const FEquipmentInfo EquipmentInfo = IEquipmentInterface::Execute_GetEquipmentInfo(EquippedTool);
		MulticastAttachActorToMainHand(EquippedTool, EquipmentInfo.MainHandSocket, EquipmentInfo.AnimationState);
	}
}

void APlayerCharacter::MulticastAttachActorToMainHand_Implementation(AActor* TargetActor, FName MainHandSocket, EAnimationState AnimationState)
{
	if(UCharacterAnimInstance* CharacterAnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance()))
	{
		CharacterAnimInstance->AnimationState = AnimationState;
	}
	if(EquippedTool) EquippedTool->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, MainHandSocket);
}

void APlayerCharacter::MulticastUnequipItem_Implementation()
{
	if(UCharacterAnimInstance* CharacterAnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance()))
	{
		CharacterAnimInstance->AnimationState =EAnimationState::Default;
	}
}

/*
 * GETTERS
 */
ATool* APlayerCharacter::GetEquippedTool()
{
	if(EquipmentComponent == nullptr) return nullptr;
	return EquipmentComponent->EquippedTool;
}

bool APlayerCharacter::IsEquipped()
{
	return (EquipmentComponent && EquipmentComponent->EquippedTool);
}

bool APlayerCharacter::IsAiming()
{
	return (EquipmentComponent && EquipmentComponent->bAiming);
}

bool APlayerCharacter::IsCasting()
{
	return bIsCasting;
}

bool APlayerCharacter::IsStunned()
{
	return bIsStunned;
}

ECombatState APlayerCharacter::GetCombatState() const
{
	if(EquipmentComponent == nullptr) return ECombatState::ECS_MAX;
	return EquipmentComponent->CombatState;
}

void APlayerCharacter::SetCombatState(const ECombatState NewState)
{
	if(EquipmentComponent == nullptr) return;
	EquipmentComponent->CombatState = NewState;
}

void APlayerCharacter::SetAnimationState(const EAnimationState NewState)
{
	if(UCharacterAnimInstance* CharacterAnimInstance = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance()))
	{
		CharacterAnimInstance->AnimationState = NewState;
	}
}

FVector APlayerCharacter::GetHitTarget() const
{
	if(EquipmentComponent == nullptr) return FVector();
	return EquipmentComponent->HitTarget;
}
