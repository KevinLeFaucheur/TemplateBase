// Retropsis @ 2024

#include "Equipment/EquipmentComponent.h"

#include "AbilitySystemComponent.h"
#include "BaseGameplayTags.h"
#include "Camera/CameraComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Equipment/Tool.h"
#include "Equipment/Data/WeapenData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Player/PlayerCharacter.h"
#include "Player/PlayerCharacterController.h"
#include "TemplateBase/TemplateBase.h"

UEquipmentComponent::UEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	BaseWalkSpeed = 600.f;;
	AimWalkSpeed = 400.f;;
}

void UEquipmentComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UEquipmentComponent, EquippedTool);
	DOREPLIFETIME(UEquipmentComponent, bAiming);
	DOREPLIFETIME(UEquipmentComponent, CombatState);
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

void UEquipmentComponent::SetHUDCarriedAmmunition()
{
	PlayerCharacterController = PlayerCharacterController == nullptr ? Cast<APlayerCharacterController>(PlayerCharacter->GetController()) : PlayerCharacterController;
	if(PlayerCharacterController)
	{
		PlayerCharacterController->SetHUDCarriedAmmunition(CarriedAmmunition);	
	}
}

/*
 * Equipping
 */
void UEquipmentComponent::EquipTool(ATool* ToolToEquip)
{
	if(PlayerCharacter == nullptr || ToolToEquip == nullptr) return;

	if(EquippedTool) EquippedTool->Drop();
	
	EquippedTool = ToolToEquip;
	EquippedTool->SetToolState(EToolState::ETS_Equipped);
	EquippedTool->SetOwner(PlayerCharacter);
	EquippedTool->SetHUDAmmunition();
	AttachToolToSocket(EquippedTool, FName("RightHandSocket"));

	if(CarriedAmmunitionMap.Contains(EquippedTool->GetToolType()))
	{
		CarriedAmmunition = CarriedAmmunitionMap[EquippedTool->GetToolType()];
	}
	SetHUDCarriedAmmunition();
	
	PlayerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
	PlayerCharacter->bUseControllerRotationYaw = true;
}

void UEquipmentComponent::OnRep_EquippedTool()
{
	if(EquippedTool && PlayerCharacter)
	{
		AttachToolToSocket(EquippedTool, FName("RightHandSocket"));
		EquippedTool->SetToolState(EToolState::ETS_Equipped);
		PlayerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
		PlayerCharacter->bUseControllerRotationYaw = true;
	}	
}

void UEquipmentComponent::AttachToolToSocket(AActor* Tool, const FName& SocketName)
{
	if(const USkeletalMeshSocket* Socket = PlayerCharacter->GetMesh()->GetSocketByName(SocketName))
	{
		Socket->AttachActor(Tool, PlayerCharacter->GetMesh());
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
	if(CombatState != ECombatState::ECS_Unoccupied) return false;

	return true;
}

void UEquipmentComponent::FireButtonPressed(bool bPressed)
{
	bFireButtonPressed = bPressed;
	if(bFireButtonPressed)
	{
		ActivateTool();
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

void UEquipmentComponent::ServerActivate_Implementation(const FVector_NetQuantize& TraceHitTarget)
{
	MulticastActivate(TraceHitTarget);
}

void UEquipmentComponent::MulticastActivate_Implementation(const FVector_NetQuantize& TraceHitTarget)
{
	if(EquippedTool == nullptr) return;
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
	PlayerCharacter->GetWorldTimerManager().SetTimer(FireIntervalTimer, this, &UEquipmentComponent::FireIntervalEnd, EquippedTool->FireInterval);
}

void UEquipmentComponent::FireIntervalEnd()
{
	if(EquippedTool == nullptr) return;
	bCanActivate = true;
	if(bFireButtonPressed && EquippedTool->bAutomatic)
	{
		ActivateTool();
	}
}

/*
 * Ammunition & Reload
*/
void UEquipmentComponent::Reload()
{
	if(CarriedAmmunition > 0 && CombatState != ECombatState::ECS_Reloading)
	{
		ServerReload();
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
		HandleReload();
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

void UEquipmentComponent::OnRep_CarriedAmmunition()
{
	PlayerCharacterController = PlayerCharacterController == nullptr ? Cast<APlayerCharacterController>(PlayerCharacter->GetController()) : PlayerCharacterController;
	if(PlayerCharacterController)
	{
		PlayerCharacterController->SetHUDCarriedAmmunition(CarriedAmmunition);	
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
				0.5f +																		// Base line for crosshairs wont overlap each other
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
	bAiming = bIsAiming;
	ServerSetAiming(bIsAiming);
	if(PlayerCharacter) PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? AimWalkSpeed : BaseWalkSpeed;
}

void UEquipmentComponent::ServerSetAiming_Implementation(bool bIsAiming)
{
	bAiming = bIsAiming;
	if(PlayerCharacter) PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? AimWalkSpeed : BaseWalkSpeed;
}
