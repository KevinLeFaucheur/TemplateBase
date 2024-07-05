// Retropsis @ 2024

#include "Equipment/EquipmentComponent.h"
#include "AbilitySystemComponent.h"
#include "BaseGameplayTags.h"
#include "AbilitySystem/BaseAbilitySystemLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Equipment/Tool.h"
#include "Data/WeapenData.h"
#include "Equipment/Weapon/Projectile.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Player/CharacterAnimInstance.h"
#include "Player/PlayerCharacter.h"
#include "Player/PlayerCharacterController.h"

UEquipmentComponent::UEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	BaseWalkSpeed = 600.f;
	AimWalkSpeed = 400.f;
}

void UEquipmentComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UEquipmentComponent, EquippedTool);
	DOREPLIFETIME(UEquipmentComponent, SecondaryTool);
	DOREPLIFETIME(UEquipmentComponent, bAiming);
	DOREPLIFETIME(UEquipmentComponent, CombatState);
	DOREPLIFETIME(UEquipmentComponent, ThrowableCount);
	DOREPLIFETIME_CONDITION(UEquipmentComponent, CarriedAmmunition, COND_OwnerOnly);
}

void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
	if(PlayerCharacter)
	{
		PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
		if(PlayerCharacter->GetFollowCamera())
		{
			DefaultFOV = PlayerCharacter->GetFollowCamera()->FieldOfView;
			CurrentFOV = DefaultFOV;
		}
	}
	if(PlayerCharacter->HasAuthority())
	{
		InitializeCarriedAmmunition();
	}
	UpdateHUDThrowableCount();
}

void UEquipmentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Autonomous Proxy only
	if(PlayerCharacter && PlayerCharacter->IsLocallyControlled())
	{
		FHitResult HitResult;
		TraceUnderCrosshairs(HitResult);
		HitTarget = HitResult.ImpactPoint;

		SetHUDCrosshairs(DeltaTime);
		InterpFOV(DeltaTime);
	}
}

/*
 * Equipping
 */
void UEquipmentComponent::EquipTool(ATool* ToolToEquip)
{
	if(PlayerCharacter == nullptr || ToolToEquip == nullptr) return;
	if(CombatState != ECombatState::ECS_Unoccupied) return;

	if(EquippedTool != nullptr && SecondaryTool == nullptr)
	{
		EquipSecondaryTool(ToolToEquip);
	}
	else
	{
		EquipPrimaryTool(ToolToEquip);		
	}
	HandleUseAimOffsets();
}

void UEquipmentComponent::EquipPrimaryTool(ATool* ToolToEquip)
{
	DropEquippedTool();
	EquippedTool = ToolToEquip;
	EquippedTool->SetToolState(EToolState::ETS_Equipped);
	EquippedTool->SetOwner(PlayerCharacter);
	AttachToolToSocket(EquippedTool, EquippedTool->GetMainHandSocket());
	PlayerCharacter->SetAnimationState(EquippedTool->GetAnimationState());

	if(EquippedTool->HasAbilities())
	{
		// Give them
		UBaseAbilitySystemLibrary::GiveToolAbilities(PlayerCharacter, PlayerCharacter->GetAbilitySystemComponent(), EquippedTool);
	}
	
	EquippedTool->SetHUDAmmunition(); // TODO: Do nothing if not Range
	UpdateCarriedAmmunition(); // TODO: Is it a FireWeapon? Add or Update Ammunition HUD
}

void UEquipmentComponent::EquipSecondaryTool(ATool* ToolToEquip)
{
	SecondaryTool = ToolToEquip;
	SecondaryTool->SetToolState(EToolState::ETS_Secondary);
	SecondaryTool->SetOwner(PlayerCharacter);
	AttachToolToSocket(SecondaryTool, SecondaryTool->GetBackSocket());

	// TODO: Could still add ammo for secondary if there is 
	// SecondaryTool->SetHUDAmmunition(); // TODO: Do nothing if not Range
	// UpdateCarriedAmmunition(); // TODO: Is it a FireWeapon? Add or Update Ammunition HUD
}

void UEquipmentComponent::OnRep_EquippedTool()
{
	if(EquippedTool && PlayerCharacter)
	{
		AttachToolToSocket(EquippedTool, EquippedTool->GetMainHandSocket());
		EquippedTool->SetToolState(EToolState::ETS_Equipped);
		EquippedTool->SetHUDAmmunition(); // TODO: Do nothing if not Range
		PlayerCharacter->SetAnimationState(EquippedTool->GetAnimationState());
		
		HandleUseAimOffsets();
	}	
}

void UEquipmentComponent::OnRep_SecondaryTool()
{
	if(SecondaryTool && PlayerCharacter)
	{
		AttachToolToSocket(SecondaryTool, SecondaryTool->GetBackSocket());
		SecondaryTool->SetToolState(EToolState::ETS_Secondary);
	}
}

/*
 * Swapping
 */
void UEquipmentComponent::SwapTools()
{
	if(CombatState != ECombatState::ECS_Unoccupied || PlayerCharacter == nullptr || !PlayerCharacter->HasAuthority()) return;

	PlayerCharacter->PlaySwapToolMontage();
	PlayerCharacter->bFinishedSwapping = false;
	CombatState = ECombatState::ECS_Swapping;
	
	ATool* TempTool = EquippedTool;
	EquippedTool = SecondaryTool;
	SecondaryTool = TempTool;
	
	if(EquippedTool->HasAbilities())
	{
		UBaseAbilitySystemLibrary::GiveToolAbilities(PlayerCharacter, PlayerCharacter->GetAbilitySystemComponent(), EquippedTool);
	}
	if(SecondaryTool->HasAbilities())
	{
		UBaseAbilitySystemLibrary::RemoveToolAbilities(PlayerCharacter, PlayerCharacter->GetAbilitySystemComponent(), SecondaryTool);
	}
}

void UEquipmentComponent::SwappingTools()
{
	EquippedTool->SetToolState(EToolState::ETS_Equipped);
	AttachToolToSocket(EquippedTool, EquippedTool->GetMainHandSocket());
	PlayerCharacter->SetAnimationState(EquippedTool->GetAnimationState());
	EquippedTool->SetHUDAmmunition(); // TODO: Do nothing if not Range
	UpdateCarriedAmmunition(); // TODO: Is it a FireWeapon? Add or Update Ammunition HUD
	
	SecondaryTool->SetToolState(EToolState::ETS_Secondary);
	AttachToolToSocket(SecondaryTool, SecondaryTool->GetBackSocket());
	
	HandleUseAimOffsets();
}

bool UEquipmentComponent::ShouldSwapTools()
{
	return EquippedTool != nullptr && SecondaryTool != nullptr;
}

void UEquipmentComponent::AttachToolToSocket(AActor* Tool, const FName& SocketName)
{
	if(PlayerCharacter == nullptr || PlayerCharacter->GetMesh() == nullptr || Tool == nullptr) return;
	if(const USkeletalMeshSocket* Socket = PlayerCharacter->GetMesh()->GetSocketByName(SocketName))
	{
		Socket->AttachActor(Tool, PlayerCharacter->GetMesh());
	}
}

void UEquipmentComponent::DropEquippedTool()
{
	if(EquippedTool) EquippedTool->Drop();
}

void UEquipmentComponent::HandleUseAimOffsets()
{
	// TODO: Does this EquippedTool orient character to Movement or ControllerOrientation
	if(EquippedTool->bUseAimOffsets)
	{
		PlayerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
		PlayerCharacter->bUseControllerRotationYaw = true;
	}
	else
	{
		PlayerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
		PlayerCharacter->bUseControllerRotationYaw = false;
	}
}

/*
 * Firing / Activation
*/
bool UEquipmentComponent::CanActivate()
{
	if(!EquippedTool) return false;
	if(EquippedTool->IsEmpty()) return false;
	if(!bCanActivate) return false;
	if(CombatState == ECombatState::ECS_Reloading && EquippedTool->CanInterruptReload()) return true; // TODO: CanInterruptReload return false if NOT Ranged

	// if(bIsLocallyReloading) return false;
	
	if(CombatState != ECombatState::ECS_Unoccupied) return false;

	return true;
}

void UEquipmentComponent::FireButtonPressed(bool bPressed)
{
	bFireButtonPressed = bPressed;
	if(bFireButtonPressed)
	{
		if(EquippedTool == nullptr) return; // TODO: Think of another place to check it
		switch (EquippedTool->GetToolClass())
		{
		case EToolClass::Tool:
			break;
		case EToolClass::HarvestingTool:
			ServerActivateTool();
			break;
		case EToolClass::MeleeWeapon:
			break;
		case EToolClass::RangeWeapon:
			ActivateTool();
			break;
		default: ;
		}
	}
}

void UEquipmentComponent::ActivateTool()
{
	if(CanActivate())
	{
		ServerActivate(HitTarget);

		if(EquippedTool)
		{
			bCanActivate = false;
			CrosshairPerShotFactor = FMath::Clamp(CrosshairPerShotFactor + 0.2f, 0.f, 1.f);
		}
		FireIntervalStart();
	}
}

void UEquipmentComponent::ServerActivateTool_Implementation()
{
	GEngine->AddOnScreenDebugMessage(159, 5.f, FColor::White, FString::Printf(TEXT("ServerActivateTool_Implementation")));
	if(PlayerCharacter && EquippedTool) IEquipmentInterface::Execute_UseItem(EquippedTool, PlayerCharacter);
	// if(PlayerCharacter && PlayerCharacter->EquippedTool) IEquipmentInterface::Execute_UseItem(PlayerCharacter->EquippedTool, PlayerCharacter);
	// MulticastActivateTool();
}

void UEquipmentComponent::MulticastActivateTool_Implementation()
{
}

void UEquipmentComponent::ServerActivate_Implementation(const FVector_NetQuantize& TraceHitTarget)
{
	MulticastActivate(TraceHitTarget);
}

void UEquipmentComponent::MulticastActivate_Implementation(const FVector_NetQuantize& TraceHitTarget)
{
	if(EquippedTool == nullptr) return;
	if(PlayerCharacter && CombatState == ECombatState::ECS_Reloading && EquippedTool->CanInterruptReload())
	{
		PlayerCharacter->PlayFireMontage(bAiming);
		EquippedTool->Activate(TraceHitTarget);
		CombatState = ECombatState::ECS_Unoccupied;
		return;
	}
	if(PlayerCharacter && CombatState == ECombatState::ECS_Unoccupied)
	{
		PlayerCharacter->PlayFireMontage(bAiming);
		EquippedTool->Activate(TraceHitTarget);
	}
}

/*
 * Automatic Fire / Activation
*/
void UEquipmentComponent::FireIntervalStart()
{
	if(EquippedTool == nullptr || PlayerCharacter == nullptr) return;
	PlayerCharacter->GetWorldTimerManager().SetTimer(FireIntervalTimer, this, &UEquipmentComponent::FireIntervalEnd, EquippedTool->GetFireInterval()); // TODO: return 0.f if not Range
}

void UEquipmentComponent::FireIntervalEnd()
{
	if(EquippedTool == nullptr) return;
	bCanActivate = true;
	if(bFireButtonPressed && EquippedTool->IsAutomatic()) // TODO: return false if not Range
	{
		ActivateTool();
	}
}

/*
 * Ammunition & Reload
*/
void UEquipmentComponent::Reload()
{
	if(CarriedAmmunition > 0 && CombatState == ECombatState::ECS_Unoccupied && EquippedTool && !EquippedTool->IsFull()) // TODO: IsFull return false if NOT Ranged
	{
		ServerReload();
	}
}

void UEquipmentComponent::ReloadSingle()
{
	if(PlayerCharacter && PlayerCharacter->HasAuthority())
	{
		UpdateSingleAmmunition();
	}
}

void UEquipmentComponent::ServerReload_Implementation()
{
	if(PlayerCharacter == nullptr || EquippedTool == nullptr) return;
	
	CombatState = ECombatState::ECS_Reloading;
	HandleReload();
}

void UEquipmentComponent::HandleReload()
{
	PlayerCharacter->PlayReloadMontage(EquippedTool->GetToolType());
}

void UEquipmentComponent::ReloadEnd()
{
	if(PlayerCharacter == nullptr) return;
	if(PlayerCharacter->HasAuthority())
	{
		CombatState = ECombatState::ECS_Unoccupied;
		UpdateAmmunition();
	}
	if(bFireButtonPressed) ActivateTool();
}

void UEquipmentComponent::UpdateAmmunition()
{
	if(PlayerCharacter == nullptr || EquippedTool == nullptr) return;
	
	const int32 ReloadAmount = AmountToReload();
	if(CarriedAmmunitionMap.Contains(EquippedTool->GetToolType()))
	{
		CarriedAmmunitionMap[EquippedTool->GetToolType()] -= ReloadAmount;
		CarriedAmmunition = CarriedAmmunitionMap[EquippedTool->GetToolType()];
	}
	SetHUDCarriedAmmunition();
	EquippedTool->AddAmmunition(ReloadAmount);
}

void UEquipmentComponent::UpdateSingleAmmunition()
{
	if(PlayerCharacter == nullptr || EquippedTool == nullptr) return;
	if(CarriedAmmunitionMap.Contains(EquippedTool->GetToolType()))
	{
		CarriedAmmunitionMap[EquippedTool->GetToolType()] -= 1;
		CarriedAmmunition = CarriedAmmunitionMap[EquippedTool->GetToolType()];
	}
	SetHUDCarriedAmmunition();
	EquippedTool->AddAmmunition(1);
	bCanActivate = true;
	if(EquippedTool->IsFull() || CarriedAmmunition == 0) JumpToReloadEnd();
}

void UEquipmentComponent::JumpToReloadEnd() const
{
	if(UCharacterAnimInstance* CharacterAnimInstance = Cast<UCharacterAnimInstance>(PlayerCharacter->GetMesh()->GetAnimInstance()))
	{
		CharacterAnimInstance->JumpToReloadEnd(EquippedTool->GetReloadEndSection());
	}
}

int32 UEquipmentComponent::AmountToReload()
{
	const int32 RoomInMag = EquippedTool->GetAmmunitionCapacity() - EquippedTool->GetAmmunition();
	if(CarriedAmmunitionMap.Contains(EquippedTool->GetToolType()))
	{
		const int32 AmountCarried =  CarriedAmmunitionMap[EquippedTool->GetToolType()];
		const int32 Least = FMath::Min(RoomInMag, AmountCarried);
		return FMath::Clamp(RoomInMag, 0, Least);
	}
	return 0;
}

void UEquipmentComponent::OnRep_CombatState()
{
	switch (CombatState) {
	case ECombatState::ECS_Unoccupied:
		if(bFireButtonPressed) ActivateTool();
		break;
	case ECombatState::ECS_Reloading:
		if(PlayerCharacter && !PlayerCharacter->IsLocallyControlled()) HandleReload();
		break;
	case ECombatState::ECS_Throwing:
		if(PlayerCharacter && !PlayerCharacter->IsLocallyControlled())
		{
			PlayerCharacter->PlayThrowMontage();
			if(EquippedTool) AttachToolToSocket(EquippedTool, EquippedTool->GetOffhandSocket());
			ToggleAttachedThrowable(true);
		}
		break;
	case ECombatState::ECS_Swapping:
		if(PlayerCharacter && !PlayerCharacter->IsLocallyControlled()) PlayerCharacter->PlaySwapToolMontage();
		break;
	default: ;
	}
}

void UEquipmentComponent::InitializeCarriedAmmunition()
{
	CarriedAmmunitionMap.Emplace(EToolType::ETT_Handgun, StartingHandgunAmmunition);
	CarriedAmmunitionMap.Emplace(EToolType::ETT_AssaultRifle, StartingAssaultRifleAmmunition);
	CarriedAmmunitionMap.Emplace(EToolType::ETT_SubMachineGun, StartingSubMachineGunAmmunition);
	CarriedAmmunitionMap.Emplace(EToolType::ETT_PumpAction, StartingShotgunAmmunition);
	CarriedAmmunitionMap.Emplace(EToolType::ETT_BoltAction, StartingHighCaliberAmmunition);
	CarriedAmmunitionMap.Emplace(EToolType::ETT_RocketLauncher, StartingRocketLauncherAmmunition);
	CarriedAmmunitionMap.Emplace(EToolType::ETT_GrenadeLauncher, StartingGrenadeLauncherAmmunition);
}

void UEquipmentComponent::UpdateCarriedAmmunition()
{
	if(EquippedTool == nullptr) return;
	if(CarriedAmmunitionMap.Contains(EquippedTool->GetToolType()))
	{
		CarriedAmmunition = CarriedAmmunitionMap[EquippedTool->GetToolType()];
	}
	SetHUDCarriedAmmunition();
}

void UEquipmentComponent::SetHUDCarriedAmmunition()
{
	PlayerCharacterController = PlayerCharacterController == nullptr ? Cast<APlayerCharacterController>(PlayerCharacter->GetController()) : PlayerCharacterController;
	if(PlayerCharacterController)
	{
		PlayerCharacterController->SetHUDCarriedAmmunition(CarriedAmmunition);	
	}
}

void UEquipmentComponent::OnRep_CarriedAmmunition()
{
	PlayerCharacterController = PlayerCharacterController == nullptr ? Cast<APlayerCharacterController>(PlayerCharacter->GetController()) : PlayerCharacterController;
	if(PlayerCharacterController)
	{
		PlayerCharacterController->SetHUDCarriedAmmunition(CarriedAmmunition);	
	}

	const bool bJumpToReloadEnd = CombatState == ECombatState::ECS_Reloading &&
		EquippedTool != nullptr &&
		EquippedTool->CanInterruptReload() &&
		CarriedAmmunition == 0;
	
	if(bJumpToReloadEnd) JumpToReloadEnd();
}

void UEquipmentComponent::PickupAmmunition(EToolType ToolType, int32 AmmunitionAmount)
{
	if(CarriedAmmunitionMap.Contains(ToolType))
	{
		CarriedAmmunitionMap[ToolType] = FMath::Clamp(CarriedAmmunitionMap[ToolType] + AmmunitionAmount, 0, MaxCarriedAmmunition);
		UpdateCarriedAmmunition();
	}
}

/*
 * Crosshairs / Aiming
 */
void UEquipmentComponent::TraceUnderCrosshairs(FHitResult& TraceHitResult)
{
	if(PlayerCharacter->GetAbilitySystemComponent() && PlayerCharacter->GetAbilitySystemComponent()->HasMatchingGameplayTag(FBaseGameplayTags::Get().Player_Block_CursorTrace))
	{
		return;
	}
	FVector2D ViewportSize;
	if(GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}
	const FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection
	);

	if(bScreenToWorld)
	{
		FVector Start = CrosshairWorldPosition;
		if(PlayerCharacter)
		{
			const float DistanceToCharacter = (PlayerCharacter->GetActorLocation() - Start).Size();
			Start += CrosshairWorldDirection * (DistanceToCharacter + TraceStartAdjustment);
			DrawDebugSphere(GetWorld(), Start, 16.f, 12, FColor::White, false);
		}
		const FVector End = Start + CrosshairWorldDirection * TRACE_LENGTH;
		GetWorld()->LineTraceSingleByChannel(TraceHitResult, Start, End, ECC_Visibility);

		if(!TraceHitResult.bBlockingHit) TraceHitResult.ImpactPoint = End;
		if(TraceHitResult.GetActor() && TraceHitResult.GetActor()->Implements<UPlayerInterface>())
		{
			HUDPackage.Color = FLinearColor::Red;
		}
		else
		{
			HUDPackage.Color = FLinearColor::White;
		}
	}
}

void UEquipmentComponent::SetHUDCrosshairs(float DeltaTime)
{
	if (PlayerCharacter == nullptr || PlayerCharacter->Controller == nullptr) return;

	PlayerCharacterController = PlayerCharacterController == nullptr ? Cast<APlayerCharacterController>(PlayerCharacter->Controller) : PlayerCharacterController;
	if(PlayerCharacterController)
	{
		PlayerHUD = PlayerHUD == nullptr ? Cast<APlayerHUD>(PlayerCharacterController->GetHUD()) : PlayerHUD;
		if(PlayerHUD)
		{
			if(EquippedTool)
			{
				HUDPackage.CrosshairsCenter = EquippedTool->CrosshairsCenter;
				HUDPackage.CrosshairsLeft = EquippedTool->CrosshairsLeft;
				HUDPackage.CrosshairsRight = EquippedTool->CrosshairsRight;
				HUDPackage.CrosshairsTop = EquippedTool->CrosshairsTop;
				HUDPackage.CrosshairsBottom = EquippedTool->CrosshairsBottom;
			}
			else
			{
				HUDPackage.CrosshairsCenter = CrosshairsCenter;
				HUDPackage.CrosshairsLeft = nullptr;
				HUDPackage.CrosshairsRight = nullptr;
				HUDPackage.CrosshairsTop = nullptr;
				HUDPackage.CrosshairsBottom = nullptr;
			}

			// Calculate Crosshairs Spread
			// [0, 600) -> [0, 1)
			const FVector2D WalkSpeedRange(0.f, PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed);
			const FVector2D VelocityMultiplierRange(0.f, 1.f);
			FVector Velocity = PlayerCharacter->GetVelocity();
			Velocity.Z = 0.f;
			
			CrosshairVelocityFactor = FMath::GetMappedRangeValueClamped(WalkSpeedRange, VelocityMultiplierRange, Velocity.Size());
			if(PlayerCharacter->GetCharacterMovement()->IsFalling())
			{
				CrosshairAirborneFactor = FMath::FInterpTo(CrosshairAirborneFactor, 2.25f, DeltaTime, 2.25f);
			}
			else
			{
				CrosshairAirborneFactor = FMath::FInterpTo(CrosshairAirborneFactor, 0.f, DeltaTime, 30.f);				
			}
			if(bAiming)
			{
				CrosshairMarksmanFactor = FMath::FInterpTo(CrosshairMarksmanFactor, 0.58, DeltaTime, 30.f);
			}
			else
			{
				CrosshairMarksmanFactor = FMath::FInterpTo(CrosshairMarksmanFactor, 0.f, DeltaTime, 30.f);			
			}
			CrosshairPerShotFactor = FMath::FInterpTo(CrosshairPerShotFactor, 0.f, DeltaTime, 4.f);
			
			HUDPackage.CrosshairSpread =
				0.5f +																		// Baseline for cross-hairs won't overlap each other
				CrosshairVelocityFactor +
				CrosshairAirborneFactor -
				CrosshairMarksmanFactor +
				CrosshairPerShotFactor;
			PlayerHUD->SetHUDPackage(HUDPackage);
		}
	}
}

/*
 * Aiming
*/
void UEquipmentComponent::InterpFOV(float DeltaTime)
{
	if(EquippedTool == nullptr) return;

	if(bAiming)
	{
		CurrentFOV = FMath::FInterpTo(CurrentFOV, EquippedTool->GetMarksmanFOV(), DeltaTime, EquippedTool->GetMarksmanInterpSpeed());
	}
	else
	{
		CurrentFOV = FMath::FInterpTo(CurrentFOV, DefaultFOV, DeltaTime, MarksmanInterpSpeed);
	}
	if(PlayerCharacter && PlayerCharacter->GetFollowCamera())
	{
		PlayerCharacter->GetFollowCamera()->SetFieldOfView(CurrentFOV);
	}
}

void UEquipmentComponent::SetAiming(bool bIsAiming)
{
	if(PlayerCharacter == nullptr || EquippedTool == nullptr) return;
	bAiming = bIsAiming;
	ServerSetAiming(bIsAiming);
	PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? AimWalkSpeed : BaseWalkSpeed;
	if(PlayerCharacter->IsLocallyControlled() && EquippedTool->HasScope()) PlayerCharacter->ToggleScopeOverlay(bIsAiming);
}

void UEquipmentComponent::ServerSetAiming_Implementation(bool bIsAiming)
{
	bAiming = bIsAiming;
	if(PlayerCharacter) PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? AimWalkSpeed : BaseWalkSpeed;
}

/*
 * Throwing
 */
void UEquipmentComponent::Throw()
{
	if(ThrowableCount == 0) return;
	if(EquippedTool == nullptr) return; // TODO: Only temporary solution, should be able to throw anyway
	if(CombatState != ECombatState::ECS_Unoccupied) return;
	CombatState = ECombatState::ECS_Throwing;
	if(PlayerCharacter)
	{
		PlayerCharacter->PlayThrowMontage();
		if(EquippedTool) AttachToolToSocket(EquippedTool, EquippedTool->GetOffhandSocket());
		ToggleAttachedThrowable(true);
	}
	if(PlayerCharacter && !PlayerCharacter->HasAuthority()) ServerThrow();
	if(PlayerCharacter && PlayerCharacter->HasAuthority())
	{
		ThrowableCount = FMath::Clamp(ThrowableCount - 1, 0 , MaxThrowableCount);
		UpdateHUDThrowableCount();
	}
}

void UEquipmentComponent::ServerThrow_Implementation()
{
	if(ThrowableCount == 0) return;
	CombatState = ECombatState::ECS_Throwing;
	if(PlayerCharacter)
	{
		PlayerCharacter->PlayThrowMontage();
		if(EquippedTool) AttachToolToSocket(EquippedTool, EquippedTool->GetOffhandSocket());
		ToggleAttachedThrowable(true);
	}
	ThrowableCount = FMath::Clamp(ThrowableCount - 1, 0 , MaxThrowableCount);
	UpdateHUDThrowableCount();
}

void UEquipmentComponent::ThrowEnd()
{
	CombatState = ECombatState::ECS_Unoccupied;
	if(EquippedTool) AttachToolToSocket(EquippedTool, EquippedTool->GetMainHandSocket());
}

void UEquipmentComponent::Throwing()
{
	ToggleAttachedThrowable(false);
	if(PlayerCharacter && PlayerCharacter->IsLocallyControlled()) ServerThrowing(HitTarget);
}

void UEquipmentComponent::ServerThrowing_Implementation(const FVector_NetQuantize& Target)
{
	if(PlayerCharacter && PlayerCharacter->GetAttachedThrowable() && ThrowableClass)
	{
		const FVector StartingLocation = PlayerCharacter->GetAttachedThrowable()->GetComponentLocation();
		const FVector ToTarget = Target - StartingLocation;
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = PlayerCharacter;
		SpawnParams.Instigator = PlayerCharacter;
		if (UWorld* World = GetWorld())
		{
			// AProjectile* Projectile = World->SpawnActor<AProjectile>(ThrowableClass, StartingLocation, ToTarget.Rotation(), SpawnParams);
			// World->SpawnActor<AProjectile>(ThrowableClass, StartingLocation, ToTarget.Rotation(), SpawnParams);
			// AProjectile* Projectile = World->SpawnActorDeferred<AProjectile>(ThrowableClass, SocketTransform, GetOwner(), InstigatorPawn, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
			// Projectile->DamageEffectSpecHandle = MakeDamageEffectSpec();
			// Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
			// Projectile->FinishSpawning(SpawnTransform);
		}
	}
}

void UEquipmentComponent::ToggleAttachedThrowable(bool bShow)
{
	if(PlayerCharacter->GetAttachedThrowable()) PlayerCharacter->GetAttachedThrowable()->SetVisibility(bShow);
}

void UEquipmentComponent::OnRep_ThrowableCount()
{
	UpdateHUDThrowableCount();
}

void UEquipmentComponent::UpdateHUDThrowableCount()
{
	PlayerCharacterController = PlayerCharacterController == nullptr ? Cast<APlayerCharacterController>(PlayerCharacter->GetController()) : PlayerCharacterController;
	if(PlayerCharacterController)
	{
		PlayerCharacterController->SetHUDThrowableCount(ThrowableCount);	
	}
}

void UEquipmentComponent::SetMeleeWeaponCollisionEnabled(const ECollisionEnabled::Type CollisionEnabled)
{
	if(PlayerCharacter->HasAuthority() && EquippedTool && EquippedTool->GetCollisionBox() && EquippedTool->GetToolClass() == EToolClass::MeleeWeapon)
	{
		EquippedTool->GetCollisionBox()->SetCollisionEnabled(CollisionEnabled);
		EquippedTool->EmptyIgnoreActors();
	}
}