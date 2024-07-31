// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_PhysicalAttack.generated.h"

/**
 * 
 */
UCLASS()
class TEMPLATEBASE_API UMMC_PhysicalAttack : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMC_PhysicalAttack();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition StrengthDef;
	FGameplayEffectAttributeCaptureDefinition DexterityDef;
};
