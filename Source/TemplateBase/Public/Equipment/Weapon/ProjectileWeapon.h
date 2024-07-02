// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "Equipment/RangeWeapon.h"
#include "ProjectileWeapon.generated.h"

class AProjectile;
/**
 * 
 */
UCLASS()
class TEMPLATEBASE_API AProjectileWeapon : public ARangeWeapon
{
	GENERATED_BODY()

public:
	virtual void Activate(const FVector& HitTarget) override;

private:
	UPROPERTY(EditAnywhere, Category="Equipment|Projectile")
	TSubclassOf<AProjectile> ProjectileClass;
};
