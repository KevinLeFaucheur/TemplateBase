// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Equipment/EquipmentComponent.h"
#include "Equipment/Tool.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

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
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	TurningInPlace = ETurningInPlace::ETIP_NotTurning;
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(APlayerCharacter, OverlappingTool, COND_OwnerOnly);
}

void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if(EquipmentComponent) EquipmentComponent->PlayerCharacter = this;
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AimOffset(DeltaTime);
}

void APlayerCharacter::Jump()
{
	if(bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		Super::Jump();
	}
}

void APlayerCharacter::AimOffset(float DeltaTime)
{
	if(EquipmentComponent && EquipmentComponent->EquippedTool == nullptr) return;
	const float Speed = UKismetMathLibrary::VSizeXY(GetVelocity());
	const bool bAirborne = GetCharacterMovement()->IsFalling();
	
	if(Speed == 0.f && !bAirborne)
	{
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
		StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		AO_Yaw = 0.f;
		bUseControllerRotationYaw = true;
		TurningInPlace = ETurningInPlace::ETIP_NotTurning;
	}
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

void APlayerCharacter::CrouchButtonPressed()
{
	bIsCrouched ? UnCrouch() : Crouch();
}

void APlayerCharacter::AimButtonPressed()
{
	if(EquipmentComponent) EquipmentComponent->SetAiming(true);
}

void APlayerCharacter::AimButtonReleased()
{
	if(EquipmentComponent) EquipmentComponent->SetAiming(false);
}

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
