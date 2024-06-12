// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/BaseGameplayAbility.h"
#include "AbilitySystem/AbilityTypes.h"
#include "Interaction/CombatInterface.h"
#include "DamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class TEMPLATEBASE_API UDamageGameplayAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);

	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor = nullptr) const;

protected:
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GAS|Abilities")
	// TMap<FGameplayTag, FDamageRange> DamageTypes;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GAS|Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GAS|Damage", meta=(Categories="Damage"))
	FGameplayTag DamageType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GAS|Damage")
	FDamageRange DamageRange;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GAS|Damage")
	float StatusEffectChance = 20.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GAS|Damage")
	float StatusEffectDamage = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GAS|Damage")
	float StatusEffectFrequency = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GAS|Damage")
	float StatusEffectDuration = 5.f;

	UFUNCTION(BlueprintPure)
	FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const;

	float GetMinDamageByDamageType(float InLevel, const FGameplayTag& DamageType);
	float GetMaxDamageByDamageType(float InLevel, const FGameplayTag& DamageType);
};
