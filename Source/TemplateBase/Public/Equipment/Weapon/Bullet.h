// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "Equipment/Weapon/Projectile.h"
#include "Bullet.generated.h"

/**
 * 
 */
UCLASS()
class TEMPLATEBASE_API ABullet : public AProjectile
{
	GENERATED_BODY()
	
public:
	ABullet();

protected:
	virtual void BeginPlay() override;
	virtual void OnHit(
		UPrimitiveComponent* HitComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse,
		const FHitResult& Hit) override;

};
