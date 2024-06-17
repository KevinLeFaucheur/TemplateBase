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

		EnhancedInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
	}
}

void APlayerCharacterController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
	UpdateMagicCircleLocation();
}

void APlayerCharacterController::CursorTrace()
{
	if(!IsValid(MagicCircle)) return;
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

void APlayerCharacterController::ClientShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit)
{
	if(IsValid(TargetCharacter) && DamageTextComponentClass/*&& IsLocalController()*/)
	{
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(DamageAmount, bBlockedHit, bCriticalHit);
	}
}

void APlayerCharacterController::UpdateMagicCircleLocation()
{
	if(IsValid(MagicCircle)/* && PlayerCharacter*/)
	{
		MagicCircle->SetActorLocation(CursorHit.ImpactPoint);
		// MagicCircle->SetActorLocation(PlayerCharacter->GetHitTarget());
	}
}

void APlayerCharacterController::ToggleMagicCircle(bool bShow, UMaterialInterface* DecalMaterial)
{
	if(bShow)
	{
		if(!IsValid(MagicCircle)/* && PlayerCharacter*/)
		{
			MagicCircle = GetWorld()->SpawnActor<AMagicCircle>(MagicCircleClass, CursorHit.ImpactPoint, FRotator::ZeroRotator);
			// MagicCircle = GetWorld()->SpawnActor<AMagicCircle>(MagicCircleClass, PlayerCharacter->GetHitTarget(), FRotator::ZeroRotator);
			if(DecalMaterial)
			{
				MagicCircle->MagicCircleDecal->SetMaterial(0, DecalMaterial);
			}
		}
	}
	else
	{
		if(IsValid(MagicCircle)) MagicCircle->Destroy();
	}
}