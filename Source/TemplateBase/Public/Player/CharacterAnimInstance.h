// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Character/CharacterData.h"
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
	
	UPROPERTY(BlueprintReadOnly, Category="PlayerCharacter")
	ETurningInPlace TurningInPlace = ETurningInPlace::ETIP_NotTurning;
	
	UPROPERTY(BlueprintReadOnly, Category="PlayerCharacter")
	FRotator RightHandRotation;

	UPROPERTY(BlueprintReadOnly, Category="PlayerCharacter")
	bool bLocallyControlled = false;

	UPROPERTY(BlueprintReadOnly, Category="PlayerCharacter")
	bool bRotateRootBone;

	UPROPERTY(BlueprintReadOnly, Category="PlayerCharacter")
	bool bIsCasting;

	/*
	 * Montages
	 */
	void PlayFireMontage(bool bIsAiming);
	void PlayHitReactMontage();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerCharacter|Montages")
	TObjectPtr<UAnimMontage> FireMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerCharacter|Montages")
	TObjectPtr<UAnimMontage> HitReactMontage;
	
private:
	FRotator CharacterRotationLastFrame;
	FRotator CharacterRotation;
	FRotator DeltaRotation;
};
