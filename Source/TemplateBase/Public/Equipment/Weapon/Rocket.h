// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "Equipment/Weapon/Projectile.h"
#include "Rocket.generated.h"

class URocketMovementComponent;
class UNiagaraComponent;
class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class TEMPLATEBASE_API ARocket : public AProjectile
{
	GENERATED_BODY()

public:
	ARocket();
	void SpawnTrailSound();

	// UPROPERTY()
	// TObjectPtr<USceneComponent> HomingTargetSceneComponent;

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	virtual void OnHit(
		UPrimitiveComponent* HitComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit) override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> TrailSound;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundAttenuation> TrailSoundAttenuation;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<URocketMovementComponent> RocketMovementComponent;

private:
	
	UPROPERTY()
	TObjectPtr<UAudioComponent> TrailSoundComponent;
};
