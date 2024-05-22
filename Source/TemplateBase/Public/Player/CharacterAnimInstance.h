// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterAnimInstance.generated.h"

class ATool;
class APlayerCharacter;
/**
 * 
 */
UCLASS()
class TEMPLATEBASE_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	UPROPERTY(BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<APlayerCharacter> PlayerCharacter;

	TObjectPtr<ATool> EquippedTool;

	UPROPERTY(BlueprintReadOnly, Category="PlayerCharacter")
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category="PlayerCharacter")
	bool bAirborne;

	UPROPERTY(BlueprintReadOnly, Category="PlayerCharacter")
	bool bIsAccelerating;
	
	UPROPERTY(BlueprintReadOnly, Category="PlayerCharacter")
	bool bIsEquipped;
	
	UPROPERTY(BlueprintReadOnly, Category="PlayerCharacter")
	bool bIsCrouched;
	
	UPROPERTY(BlueprintReadOnly, Category="PlayerCharacter")
	bool bAiming;

	UPROPERTY(BlueprintReadOnly, Category="PlayerCharacter")
	float YawOffset;

	UPROPERTY(BlueprintReadOnly, Category="PlayerCharacter")
	float Lean;

	UPROPERTY(BlueprintReadOnly, Category="PlayerCharacter")
	float AO_Yaw;

	UPROPERTY(BlueprintReadOnly, Category="PlayerCharacter")
	float AO_Pitch;

	UPROPERTY(BlueprintReadOnly, Category="PlayerCharacter")
	FTransform LeftHandTransform;
	
private:
	FRotator CharacterRotationLastFrame;
	FRotator CharacterRotation;
	FRotator DeltaRotation;
	
private:
};
