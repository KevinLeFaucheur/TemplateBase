// Retropsis @ 2024

#include "Player/CharacterAnimInstance.h"

#include "Equipment/Tool.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/PlayerCharacter.h"

void UCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
}

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(!IsValid(PlayerCharacter))
	{
		PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
	}
	if(!IsValid(PlayerCharacter)) return;;

	Speed = UKismetMathLibrary::VSizeXY(PlayerCharacter->GetVelocity());
	bAirborne = PlayerCharacter->GetCharacterMovement()->IsFalling();
	bIsAccelerating = UKismetMathLibrary::NotEqual_VectorVector(PlayerCharacter->GetCharacterMovement()->GetCurrentAcceleration(), FVector::ZeroVector, 0.f)
		&& Speed > 3.f;
	bIsEquipped = PlayerCharacter->IsEquipped();
	bIsCrouched = PlayerCharacter->bIsCrouched;
	bAiming = PlayerCharacter->IsAiming();
	TurningInPlace = PlayerCharacter->GetTurningInPlace();
	EquippedTool = PlayerCharacter->GetEquippedTool();

	/* YawOffset */
	const FRotator AimRotation = PlayerCharacter->GetBaseAimRotation();
	const FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(PlayerCharacter->GetVelocity());
	YawOffset = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw;

	/* This might not be needed with Wrap Input checked */ 
	const FRotator DeltaOffset = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation);
	DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaOffset, DeltaSeconds, 6.f);
	YawOffset = DeltaRotation.Yaw;
	//
	
	if(!PlayerCharacter->HasAuthority())
	{
		GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, FString::Printf(TEXT("AimRotation: %f"), AimRotation.Yaw));
		GEngine->AddOnScreenDebugMessage(2, 1.f, FColor::Green, FString::Printf(TEXT("MovementRotation: %f"), MovementRotation.Yaw));
	}

	/* Lean */
	CharacterRotationLastFrame = CharacterRotation;
	CharacterRotation = PlayerCharacter->GetActorRotation();
	const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);
	const float Target = Delta.Yaw / DeltaSeconds;
	const float Interp = FMath::FInterpTo(Lean, Target, DeltaSeconds, 6.f);
	Lean = FMath::Clamp(Interp, -90.f, 90.f);

	/* AO_Yaw */
	AO_Yaw = PlayerCharacter->GetAO_Yaw();
	AO_Pitch = PlayerCharacter->GetAO_Pitch();

	/* FABRIK */
	if(bIsEquipped && EquippedTool && EquippedTool->GetMesh() && PlayerCharacter->GetMesh())
	{
		LeftHandTransform = EquippedTool->GetMesh()->GetSocketTransform(FName("LeftHandSocket"), RTS_World);
		FVector OutPosition;
		FRotator OutRotation;
		PlayerCharacter->GetMesh()->TransformToBoneSpace(FName("hand_r"), LeftHandTransform.GetLocation(), FRotator::ZeroRotator, OutPosition, OutRotation);
		LeftHandTransform.SetLocation(OutPosition);
		LeftHandTransform.SetRotation(FQuat(OutRotation));
	}
}
