// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterAnimInstance.generated.h"

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
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category="PlayerCharacter")
	bool bAirborne;

	UPROPERTY(BlueprintReadOnly, Category="PlayerCharacter")
	bool bIsAccelerating;
	
private:
};
