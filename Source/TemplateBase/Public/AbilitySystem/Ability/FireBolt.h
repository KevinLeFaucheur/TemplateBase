// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/ProjectileSpell.h"
#include "FireBolt.generated.h"

/**
 * 
 */
UCLASS()
class TEMPLATEBASE_API UFireBolt : public UProjectileSpell
{
	GENERATED_BODY()

public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;

	UFUNCTION(BlueprintCallable)
	void SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& CombatSocketTag, bool bOverridePitch = false, float PitchOverride = 0.f, AActor* HomingTarget = nullptr);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="Ability|Projectile")
	float ProjectileSpread = 90.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Ability|Projectile")
	int32 MaxNumProjectiles = 5;
	
	UPROPERTY(EditDefaultsOnly, Category="Ability|Projectile")
	float HomingAccelerationMin = 1600.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Ability|Projectile")
	float HomingAccelerationMax = 3200.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Ability|Projectile")
	bool bLaunchHomingProjectiles = true;
};
