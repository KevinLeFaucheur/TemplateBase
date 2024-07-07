// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Equipment/EquipmentData.h"
#include "Character/CharacterData.h"
#include "CharacterAnimInstance.generated.h"

enum class EToolType : uint8;
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

	UPROPERTY(BlueprintReadOnly, Category="PlayerCharacter")
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
	ECombatState CombatState = ECombatState::ECS_Unoccupied;
	
	UPROPERTY(BlueprintReadOnly, Category="PlayerCharacter")
	EAnimationState AnimationState = EAnimationState::Default;
	
	UPROPERTY(BlueprintReadOnly, Category="PlayerCharacter")
	FRotator RightHandRotation;

	UPROPERTY(BlueprintReadOnly, Category="PlayerCharacter")
	bool bLocallyControlled = false;

	UPROPERTY(BlueprintReadOnly, Category="PlayerCharacter")
	bool bRotateRootBone;

	UPROPERTY(BlueprintReadOnly, Category="PlayerCharacter")
	bool bIsCasting;

	UPROPERTY(BlueprintReadOnly, Category="PlayerCharacter")
	bool bIsStunned;

	UPROPERTY(BlueprintReadOnly, Category="PlayerCharacter")
	bool bUseFABRIK;

	UPROPERTY(BlueprintReadOnly, Category="PlayerCharacter")
	bool bUseAimOffset;

	UPROPERTY(BlueprintReadOnly, Category="PlayerCharacter")
	bool bTransformRightHand;

	/*
	 * Montages
	 */
	void PlayFireMontage(bool bIsAiming);
	void PlayHitReactMontage();
	void PlayThrowMontage();
	void PlaySwapToolMontage();
	void PlayReloadMontage(const EToolType ToolType);
	void JumpToReloadEnd(const FName& SectionName);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerCharacter|Montages")
	TObjectPtr<UAnimMontage> FireMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerCharacter|Montages")
	TObjectPtr<UAnimMontage> HitReactMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerCharacter|Montages")
	TObjectPtr<UAnimMontage> ReloadMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerCharacter|Montages")
	TObjectPtr<UAnimMontage> ThrowMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerCharacter|Montages")
	TObjectPtr<UAnimMontage> SwapToolMontage;

	UPROPERTY(EditAnywhere, Category="PlayerCharacter|IK")
	bool bDebugLineTrace = false;
	
private:
	FRotator CharacterRotationLastFrame;
	FRotator CharacterRotation;
	FRotator DeltaRotation;

public:
	FORCEINLINE UAnimMontage* GetReloadMontage() const { return ReloadMontage; };
};
