// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/PlayerCharacterController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "BaseGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "Actor/MagicCircle.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerCharacter.h"
#include "Player/Input/BaseEnhancedInputComponent.h"
#include "TemplateBase/TemplateBase.h"
#include "UI/DamageTextComponent.h"
#include "UI/HealingTextComponent.h"

void APlayerCharacterController::BeginPlay()
{
	Super::BeginPlay();
	PlayerCharacter = Cast<APlayerCharacter>(GetCharacter());
}

void APlayerCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
	
	if (UBaseEnhancedInputComponent* EnhancedInputComponent = Cast<UBaseEnhancedInputComponent>(InputComponent)) {
		
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacterController::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacterController::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacterController::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &APlayerCharacterController::StopJumping);
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Started, this, &APlayerCharacterController::EquipButtonPressed);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &APlayerCharacterController::CrouchButtonPressed);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &APlayerCharacterController::FireButtonPressed);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &APlayerCharacterController::FireButtonReleased);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &APlayerCharacterController::AimButtonPressed);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &APlayerCharacterController::AimButtonReleased);
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &APlayerCharacterController::ReloadButtonPressed);
		EnhancedInputComponent->BindAction(ThrowAction, ETriggerEvent::Started, this, &APlayerCharacterController::ThrowButtonPressed);
		EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Started, this, &APlayerCharacterController::InventoryButtonPressed);
		
		EnhancedInputComponent->BindAction(ToggleMouseCursorAction, ETriggerEvent::Started, this, &APlayerCharacterController::ToggleMouseCursorPressed);

		/*
		 * Hotbar
		 */
		EnhancedInputComponent->BindAction(HotbarAction_0, ETriggerEvent::Started, this, &APlayerCharacterController::HotbarButtonPressed, 9);
		// EnhancedInputComponent->BindAction(HotbarAction_1, ETriggerEvent::Started, this, &APlayerCharacterController::HotbarButtonPressed, 0);
		// EnhancedInputComponent->BindAction(HotbarAction_2, ETriggerEvent::Started, this, &APlayerCharacterController::HotbarButtonPressed, 1);
		// EnhancedInputComponent->BindAction(HotbarAction_3, ETriggerEvent::Started, this, &APlayerCharacterController::HotbarButtonPressed, 2);
		// EnhancedInputComponent->BindAction(HotbarAction_4, ETriggerEvent::Started, this, &APlayerCharacterController::HotbarButtonPressed, 3);
		EnhancedInputComponent->BindAction(HotbarAction_5, ETriggerEvent::Started, this, &APlayerCharacterController::HotbarButtonPressed, 0);
		EnhancedInputComponent->BindAction(HotbarAction_6, ETriggerEvent::Started, this, &APlayerCharacterController::HotbarButtonPressed, 1);
		EnhancedInputComponent->BindAction(HotbarAction_7, ETriggerEvent::Started, this, &APlayerCharacterController::HotbarButtonPressed, 2);
		EnhancedInputComponent->BindAction(HotbarAction_8, ETriggerEvent::Started, this, &APlayerCharacterController::HotbarButtonPressed, 3);
		EnhancedInputComponent->BindAction(HotbarAction_9, ETriggerEvent::Started, this, &APlayerCharacterController::HotbarButtonPressed, 4);

		EnhancedInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
	}
}

void APlayerCharacterController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	
	if(IsValid(MagicCircle))
	{
		CursorTrace();
		UpdateMagicCircleLocation();
	}
}

void APlayerCharacterController::CursorTrace()
{
	if(PlayerCharacter->GetAbilitySystemComponent() && PlayerCharacter->GetAbilitySystemComponent()->HasMatchingGameplayTag(FBaseGameplayTags::Get().Player_Block_CursorTrace))
	{
		return;
	}
	
	const ECollisionChannel TraceChannel = IsValid(MagicCircle) ? ECC_ExcludeTargets : ECC_Visibility;
	FVector2D ViewportSize;
	if(GEngine && GEngine->GameViewport) GEngine->GameViewport->GetViewportSize(ViewportSize);
	
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
		GetWorld()->LineTraceSingleByChannel(CursorHit, Start, End, TraceChannel);

		if(!CursorHit.bBlockingHit) CursorHit.ImpactPoint = End;
	}
}

void APlayerCharacterController::Move(const FInputActionValue& Value)
{
	if(GetASC() && GetASC()->HasMatchingGameplayTag(FBaseGameplayTags::Get().Player_Block_InputPressed)) return;
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		ControlledPawn->AddMovementInput(ForwardDirection, MovementVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APlayerCharacterController::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddControllerYawInput(LookAxisVector.X);
		ControlledPawn->AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APlayerCharacterController::Jump()
{
	if (PlayerCharacter) PlayerCharacter->Jump();
}

void APlayerCharacterController::StopJumping()
{
	if (PlayerCharacter) PlayerCharacter->StopJumping();
}

void APlayerCharacterController::CrouchButtonPressed()
{
	if (PlayerCharacter) PlayerCharacter->CrouchButtonPressed();
}

void APlayerCharacterController::EquipButtonPressed()
{
	if (PlayerCharacter) PlayerCharacter->EquipButtonPressed();
}

void APlayerCharacterController::FireButtonPressed()
{
	if (PlayerCharacter) PlayerCharacter->FireButtonPressed();
}

void APlayerCharacterController::FireButtonReleased()
{
	if (PlayerCharacter) PlayerCharacter->FireButtonReleased();
}

void APlayerCharacterController::AimButtonPressed()
{
	if (PlayerCharacter) PlayerCharacter->AimButtonPressed();
}

void APlayerCharacterController::AimButtonReleased()
{
	if (PlayerCharacter) PlayerCharacter->AimButtonReleased();
}

void APlayerCharacterController::ReloadButtonPressed()
{
	if (PlayerCharacter) PlayerCharacter->ReloadButtonPressed();
}

void APlayerCharacterController::ThrowButtonPressed()
{
	if (PlayerCharacter) PlayerCharacter->ThrowButtonPressed();
}

void APlayerCharacterController::ToggleMouseCursorPressed()
{
	SetShowMouseCursor(!ShouldShowMouseCursor());
	if(ShouldShowMouseCursor())
	{
		const FInputModeGameAndUI InputModeGameAndUI;
		SetInputMode(InputModeGameAndUI);
	}
	else
	{
		const FInputModeGameOnly InputModeGameOnly;
		SetInputMode(InputModeGameOnly);		
	}
}

void APlayerCharacterController::InventoryButtonPressed_Implementation()
{
	ToggleInventory();
}

/*
 * Hotbar
*/
void APlayerCharacterController::HotbarButtonPressed(int32 Index)
{
	if (PlayerCharacter) PlayerCharacter->ServerUseHotbarSlot(Index);
}

/*
 * Ability
 */
void APlayerCharacterController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if(GetASC() && GetASC()->HasMatchingGameplayTag(FBaseGameplayTags::Get().Player_Block_InputPressed)) return;
	if(GetASC() == nullptr) return;
	GetASC()->AbilityInputTagPressed(InputTag);
}

void APlayerCharacterController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if(GetASC() && GetASC()->HasMatchingGameplayTag(FBaseGameplayTags::Get().Player_Block_InputReleased)) return;
	if(GetASC() == nullptr) return;
	GetASC()->AbilityInputTagReleased(InputTag);
}

void APlayerCharacterController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if(GetASC() && GetASC()->HasMatchingGameplayTag(FBaseGameplayTags::Get().Player_Block_InputHeld)) return;
	if(GetASC() == nullptr) return;
	GetASC()->AbilityInputTagHeld(InputTag);
}

UBaseAbilitySystemComponent* APlayerCharacterController::GetASC()
{
	if(BaseAbilitySystemComponent == nullptr)
	{
		BaseAbilitySystemComponent = Cast<UBaseAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return BaseAbilitySystemComponent;
}

void APlayerCharacterController::ClientShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit, float Delay)
{
	// TODO: Probably want some kind of option to disable delay
	if(IsValid(TargetCharacter) && DamageTextComponentClass/*&& IsLocalController()*/)
	{
		if(Delay <= 0.f) ShowDamageNumber(DamageAmount, TargetCharacter, bBlockedHit, bCriticalHit);
		else
		{
			FTimerHandle DamageDelay;
			FTimerDelegate TimerDelegate;
			TimerDelegate.BindUObject(this, &APlayerCharacterController::ShowDamageNumber, DamageAmount, TargetCharacter, bBlockedHit, bCriticalHit);
			GetWorldTimerManager().SetTimer(DamageDelay, TimerDelegate, Delay, false);
		}
	}
}

void APlayerCharacterController::ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit) const
{
	UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
	DamageText->RegisterComponent();
	DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	const FVector DamageTextLocation = DamageText->GetComponentLocation();
	const float RandY = DamageTextLocation.Y + FMath::FRandRange(- 50.f, 50.f); 
	const float RandZ = DamageTextLocation.Z + FMath::FRandRange(0.f, 75.f);
	DamageText->SetWorldLocation(FVector(DamageTextLocation.X, RandY, RandZ));
	DamageText->SetDamageText(DamageAmount, bBlockedHit, bCriticalHit);
}

void APlayerCharacterController::ClientShowHealingNumber_Implementation(float HealingAmount, ACharacter* TargetCharacter, bool bCriticalHit, float Delay)
{
	// TODO: Probably want some kind of option to disable delay
	if(IsValid(TargetCharacter) && HealingTextComponentClass/*&& IsLocalController()*/)
	{
		if(Delay <= 0.f) ShowHealingNumber(HealingAmount, TargetCharacter, bCriticalHit);
		else
		{
			FTimerHandle DamageDelay;
			FTimerDelegate TimerDelegate;
			TimerDelegate.BindUObject(this, &APlayerCharacterController::ShowHealingNumber, HealingAmount, TargetCharacter, bCriticalHit);
			GetWorldTimerManager().SetTimer(DamageDelay, TimerDelegate, Delay, false);
		}
	}
}

void APlayerCharacterController::ShowHealingNumber(float HealingAmount, ACharacter* TargetCharacter, bool bCriticalHit) const
{
	UHealingTextComponent* HealingText = NewObject<UHealingTextComponent>(TargetCharacter, HealingTextComponentClass);
	HealingText->RegisterComponent();
	HealingText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	HealingText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	// const FVector HealingTextLocation = HealingText->GetComponentLocation();
	// const float RandY = HealingTextLocation.Y + FMath::FRandRange(- 50.f, 50.f); 
	// const float RandZ = HealingTextLocation.Z + FMath::FRandRange(0.f, 75.f);
	// HealingText->SetWorldLocation(FVector(HealingTextLocation.X, RandY, RandZ));
	HealingText->SetHealingText(HealingAmount, bCriticalHit);
}

void APlayerCharacterController::UpdateMagicCircleLocation()
{
	if(IsValid(MagicCircle)) MagicCircle->SetActorLocation(CursorHit.ImpactPoint);
}

void APlayerCharacterController::SetHUDAmmunition(int32 Ammo) const
{
	AmmunitionChanged.Broadcast(Ammo);
}

void APlayerCharacterController::SetHUDCarriedAmmunition(int32 Ammo) const
{
	CarriedAmmunitionChanged.Broadcast(Ammo);
}

void APlayerCharacterController::SetHUDThrowableCount(int32 GrenadeCount) const
{
	ThrowableCountChanged.Broadcast(GrenadeCount);
}

void APlayerCharacterController::ToggleMagicCircle(bool bShow, UMaterialInterface* DecalMaterial)
{
	if(bShow)
	{
		if(!IsValid(MagicCircle))
		{
			MagicCircle = GetWorld()->SpawnActor<AMagicCircle>(MagicCircleClass, CursorHit.ImpactPoint, FRotator::ZeroRotator);
			if(DecalMaterial) MagicCircle->MagicCircleDecal->SetMaterial(0, DecalMaterial);
		}
	}
	else if(IsValid(MagicCircle)) MagicCircle->Destroy();
}

void APlayerCharacterController::UpdateInventorySlot_Implementation(EContainerType ContainerType, int32 SlotIndex,
	FInventoryItemData ItemData)
{
}