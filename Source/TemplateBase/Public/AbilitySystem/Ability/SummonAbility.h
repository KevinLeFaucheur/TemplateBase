// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/BaseGameplayAbility.h"
#include "SummonAbility.generated.h"

/**
 * 
 */
UCLASS()
class TEMPLATEBASE_API USummonAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category="GAS|Abilities|Summon")
	TArray<FVector> GetSpawnLocations();

	UFUNCTION(BlueprintPure, Category="GAS|Abilities|Summon")
	TSubclassOf<APawn> GetRandomMinionClass();

	UPROPERTY(EditDefaultsOnly, Category="GAS|Abilities|Summon")
	int32 NumMinions = 5;

	UPROPERTY(EditDefaultsOnly, Category="GAS|Abilities|Summon")
	TArray<TSubclassOf<APawn>> MinionClasses;

	UPROPERTY(EditDefaultsOnly, Category="GAS|Abilities|Summon")
	float MinSpawnDistance = 150.f;
	
	UPROPERTY(EditDefaultsOnly, Category="GAS|Abilities|Summon")
	float MaxSpawnDistance = 450.f;

	UPROPERTY(EditDefaultsOnly, Category="GAS|Abilities|Summon")
	float SpawnSpread = 90.f;
};
