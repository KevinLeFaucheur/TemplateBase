// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "Equipment/Weapon.h"
#include "HitScanWeapon.generated.h"

/**
 * 
 */
UCLASS()
class TEMPLATEBASE_API AHitScanWeapon : public AWeapon
{
	GENERATED_BODY()

public:
	void ApplyDamage(AController* InstigatorController, const FHitResult& FireHit);
	void PlayHitEffects(const UWorld* World, const FHitResult& FireHit) const;
	void ShowTracer(const FTransform& SocketTransform, UWorld* World, const FVector& BeamEnd) const;
	virtual void Activate(const FVector& HitTarget) override;

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
