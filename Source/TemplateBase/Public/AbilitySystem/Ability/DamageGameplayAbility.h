// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/BaseGameplayAbility.h"
#include "ScalableFloat.h"
#include "Interaction/CombatInterface.h"
#include "DamageGameplayAbility.generated.h"

USTRUCT(BlueprintType)
struct FDamageRange
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GAS|Meta")
	FScalableFloat DamageMin = FScalableFloat();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GAS|Meta")
	FScalableFloat DamageMax = FScalableFloat();
};

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

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GAS|Abilities")
	TMap<FGameplayTag, FDamageRange> DamageTypes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GAS|Meta")
	FDamageRange DamageRange;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GAS|Abilities")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UFUNCTION(BlueprintPure)
	FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const;
};
