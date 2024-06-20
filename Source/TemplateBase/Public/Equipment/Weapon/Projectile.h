// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/AbilityTypes.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class UProjectileMovementComponent;
class UBoxComponent;

UCLASS()
class TEMPLATEBASE_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectile();
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;

protected:
	virtual void BeginPlay() override;
	bool IsValidOverlap(AActor* OtherActor);
	void PlayImpactEffects();
	
	UFUNCTION()
	virtual void OnHit(
		UPrimitiveComponent* HitComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit
		);
	
	UPROPERTY(VisibleAnywhere, Category="Weapon|Projectile")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;
	
	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn))
	FDamageEffectParams DamageEffectParams;
	
	UPROPERTY(EditAnywhere, Category="Weapon|Projectile")
	TObjectPtr<UBoxComponent> HitCollision;
	
	UPROPERTY(EditAnywhere, Category="Weapon|Projectile")
	float InitialSpeed = 15000.f;

private:

	UPROPERTY(EditAnywhere, Category="Weapon|Projectile|Effects")
	TObjectPtr<UParticleSystem> Tracer;
	
	TObjectPtr<UParticleSystemComponent> TracerComponent;

	UPROPERTY(EditAnywhere, Category="Weapon|Projectile|Effects")
	TObjectPtr<UParticleSystem> ImpactParticles;

	UPROPERTY(EditAnywhere, Category="Weapon|Projectile|Effects")
	TObjectPtr<USoundBase> ImpactSound;
};
