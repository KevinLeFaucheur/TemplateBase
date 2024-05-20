// Retropsis @ 2024

#include "Player/CharacterAnimInstance.h"
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
	bIsAccelerating =
		UKismetMathLibrary::NotEqual_VectorVector(PlayerCharacter->GetCharacterMovement()->GetCurrentAcceleration(), FVector::ZeroVector, 0.f)
		&& Speed > 3.f;
}
