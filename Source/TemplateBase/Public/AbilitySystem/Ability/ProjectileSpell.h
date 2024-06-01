// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/BaseGameplayAbility.h"
#include "ProjectileSpell.generated.h"

class ASpellProjectile;
/**
 * 
 */
UCLASS()
class TEMPLATEBASE_API UProjectileSpell : public UBaseGameplayAbility
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable, Category="GAS|Abilities")
	void SpawnProjectile(const FVector& ProjectileTargetLocation);
	
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GAS|Abilities")
	TSubclassOf<ASpellProjectile> SpellProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GAS|Abilities")
	TSubclassOf<UGameplayEffect> DamageEffectClass;
};
