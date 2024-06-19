// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "Equipment/Weapon/Projectile.h"
#include "Rocket.generated.h"

/**
 * 
 */
UCLASS()
class TEMPLATEBASE_API ARocket : public AProjectile
{
	GENERATED_BODY()

public:
	ARocket();
	
	// UPROPERTY()
	// TObjectPtr<USceneComponent> HomingTargetSceneComponent;

protected:
	virtual void OnHit(
		UPrimitiveComponent* HitComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit) override;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> RocketMesh;
};
