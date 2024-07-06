// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/DamageGameplayAbility.h"
#include "ThrowAbility.generated.h"

class AProjectile;
/**
 * 
 */
UCLASS()
class TEMPLATEBASE_API UThrowAbility : public UDamageGameplayAbility
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable, Category="GAS|Abilities")
	void SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& CombatSocketTag, bool bOverridePitch = false, float PitchOverride = 0.f);
	
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GAS|Abilities")
	TSubclassOf<AProjectile> ThrowableClass;

	UPROPERTY(EditDefaultsOnly)
	int32 NumThrowables = 5;

	UFUNCTION(BlueprintCallable, Category="GAS|Abilities")
	bool CanThrow() const;

	UFUNCTION(BlueprintCallable, Category="GAS|Abilities")
	void UseAvailableThrowable() const;

	FVector GetSpawnLocation() const;
	FRotator GetSpawnRotation() const;
	FVector SpawnLocation;
	FRotator SpawnDirection;
	float StartTraceOffset = 480.f;
	float DefaultTargetingRange = 3150.f;
	
};
