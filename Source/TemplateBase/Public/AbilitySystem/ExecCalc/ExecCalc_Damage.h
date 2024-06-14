// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ExecCalc_Damage.generated.h"

/**
 * 
 */
UCLASS()
class TEMPLATEBASE_API UExecCalc_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UExecCalc_Damage();
	void DetermineStatusEffect(
		const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	    const FGameplayEffectSpec& Spec,
	    FAggregatorEvaluateParameters EvaluationParameters,
	    const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& TagsToDefs) const;

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
