// Retropsis @ 2024

#include "Equipment/EquipmentComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Equipment/Tool.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Player/PlayerCharacter.h"
#include "Player/PlayerCharacterController.h"

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

	EquippedTool = ToolToEquip;
	EquippedTool->SetToolState(EToolState::ETS_Equipped);
	EquippedTool->SetOwner(PlayerCharacter);
	if(const USkeletalMeshSocket* HandSocket = PlayerCharacter->GetMesh()->GetSocketByName(FName("RightHandSocket")))
	{
		HandSocket->AttachActor(EquippedTool, PlayerCharacter->GetMesh());
	}
	PlayerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
	PlayerCharacter->bUseControllerRotationYaw = true;
}

void UEquipmentComponent::OnRep_EquippedTool()
{
	if(EquippedTool && PlayerCharacter)
	{
		PlayerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
		PlayerCharacter->bUseControllerRotationYaw = true;
	}	
}

/*
 * Firing
 */
void UEquipmentComponent::FireButtonPressed(bool bPressed)
{
	bFireButtonPressed = bPressed;
	if(bFireButtonPressed)
	{
		FHitResult HitResult;
		TraceUnderCrosshairs(HitResult);
		ServerActivate(HitResult.ImpactPoint);

		if(EquippedTool)
		{
			CrosshairPerShotFactor = FMath::Clamp(CrosshairPerShotFactor + 0.2f, 0.f, 1.f);
		}
	}
}

void UEquipmentComponent::ServerActivate_Implementation(const FVector_NetQuantize& TraceHitTarget)
{
	MulticastFire(TraceHitTarget);
}

void UEquipmentComponent::MulticastFire_Implementation(const FVector_NetQuantize& TraceHitTarget)
{
	if(EquippedTool == nullptr) return;
	if(PlayerCharacter)
	{
		PlayerCharacter->PlayFireMontage(bAiming);
		EquippedTool->Activate(TraceHitTarget);
	}
}

void UEquipmentComponent::TraceUnderCrosshairs(FHitResult& TraceHitResult)
{
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
		const FVector Start = CrosshairWorldPosition;
		const FVector End = Start + CrosshairWorldDirection * TRACE_LENGTH;
		GetWorld()->LineTraceSingleByChannel(TraceHitResult, Start, End, ECC_Visibility);

		if(!TraceHitResult.bBlockingHit) TraceHitResult.ImpactPoint = End;
		if(TraceHitResult.GetActor() &&TraceHitResult.GetActor()->Implements<UPlayerInterface>())
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
