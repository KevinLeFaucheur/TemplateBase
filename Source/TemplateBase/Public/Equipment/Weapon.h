// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "Equipment/Tool.h"
#include "Weapon.generated.h"

/**
 * 
 */
UCLASS()
class TEMPLATEBASE_API AWeapon : public ATool
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void ApplyHitScanDamage(AActor* TargetActor) const;
	FGameplayEffectSpecHandle MakeDamageEffectSpec() const;

	UFUNCTION(BlueprintPure)
	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(
		AActor* TargetActor = nullptr,
		FVector InRadialDamageOrigin = FVector::ZeroVector,
		bool bOverrideAirborneDirection = false,
		FVector AirborneDirectionOverride = FVector::ZeroVector,
		bool bOverrideDeathImpulseDirection = false,
		FVector DeathImpulseDirectionOverride = FVector::ZeroVector,
		bool bOverridePitch = false,
		float PitchOverride = 0.f) const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Damage")
	bool bIsRadialDamage = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Damage")
	float RadialDamageInnerRadius = 0.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Damage")
	float RadialDamageOuterRadius = 0.f;
};
