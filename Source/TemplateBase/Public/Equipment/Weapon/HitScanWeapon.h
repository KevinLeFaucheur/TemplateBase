// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "Equipment/RangeWeapon.h"
#include "HitScanWeapon.generated.h"

/**
 * 
 */
UCLASS()
class TEMPLATEBASE_API AHitScanWeapon : public ARangeWeapon
{
	GENERATED_BODY()

public:
	virtual void Activate(const FVector& HitTarget) override;
	void WeaponTraceHit(const FVector& TraceStart, const FVector& HitTarget, FHitResult& OutHit) const;
	void ApplyDamage(const FHitResult& FireHit);
	void PlayHitEffects(const FHitResult& FireHit, bool bOverrideMultipliers = false) const;
	void ShowTracer(const FVector& AtLocation, const UWorld* World, const FVector& BeamEnd) const;

private:
	UPROPERTY(EditAnywhere, Category="Equipment|HitScan")
	float Damage = 20.f;

	UPROPERTY(EditAnywhere, Category="Equipment|HitScan")
	TObjectPtr<UParticleSystem> ImpactParticles;

	UPROPERTY(EditAnywhere, Category="Equipment|HitScan")
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(EditAnywhere, Category="Equipment|HitScan")
	TObjectPtr<UParticleSystem> BeamParticles;
};
