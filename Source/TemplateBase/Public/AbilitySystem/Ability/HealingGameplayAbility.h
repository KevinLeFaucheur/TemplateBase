// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/AbilityTypes.h"
#include "AbilitySystem/Ability/BaseGameplayAbility.h"
#include "HealingGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class TEMPLATEBASE_API UHealingGameplayAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SingleTargetHeal(AActor* TargetActor) const;
	
	UFUNCTION(BlueprintPure)
	float GetHealingPowerAtLevel() const;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GAS|Healing")
	TSubclassOf<UGameplayEffect>HealingEffectClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GAS|Healing", meta=(Categories="Healing"))
	FGameplayTag HealingType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GAS|Healing")
	FDamageRange HealingRange;
	
	float GetMinHealingByLevel(float InLevel) const;
	float GetMaxHealingByLevel(float InLevel) const;
};
