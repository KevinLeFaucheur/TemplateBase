// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "DamageGameplayAbility.h"
#include "ProjectileSpell.generated.h"

class ASpellProjectile;
/**
 * 
 */
UCLASS()
class TEMPLATEBASE_API UProjectileSpell : public UDamageGameplayAbility
{
	GENERATED_BODY()

public:
	// virtual FString GetDescription(int32 Level) override;
	// virtual FString GetNextLevelDescription(int32 Level) override;

protected:
	UFUNCTION(BlueprintCallable, Category="GAS|Abilities")
	void SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& CombatSocketTag, bool bOverridePitch = false, float PitchOverride = 0.f);
	
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GAS|Abilities")
	TSubclassOf<ASpellProjectile> SpellProjectileClass;

	UPROPERTY(EditDefaultsOnly)
	int32 NumProjectiles = 5;
};
