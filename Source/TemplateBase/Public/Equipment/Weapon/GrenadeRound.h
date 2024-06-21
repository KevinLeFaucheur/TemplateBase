// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "Equipment/Weapon/Projectile.h"
#include "GrenadeRound.generated.h"

/**
 * 
 */
UCLASS()
class TEMPLATEBASE_API AGrenadeRound : public AProjectile
{
	GENERATED_BODY()

public:
	AGrenadeRound();
	virtual void Destroyed() override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity);

private:
	UPROPERTY(EditAnywhere, Category="Equipment|GrenadeRound")
	TObjectPtr<USoundBase> BouncingSound;
};
