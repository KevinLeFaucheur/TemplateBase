// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/AbilityTypes.h"
#include "GameFramework/Actor.h"
#include "SpellProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UNiagaraSystem;

UCLASS()
class TEMPLATEBASE_API ASpellProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpellProjectile();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn))
	FDamageEffectParams DamageEffectParams;

	UPROPERTY()
	TObjectPtr<USceneComponent> HomingTargetSceneComponent;

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	UFUNCTION(BlueprintCallable)
	virtual void OnHit();

	bool IsValidOverlap(AActor* OtherActor);
	void ApplyDamageEffects(AActor* OtherActor);

	UFUNCTION()
	virtual void OnSphereBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent*OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	
	bool bHit = false;

private:
	UPROPERTY(EditDefaultsOnly, Category="GAS|Abilities")
	float LifeSPan = 15.f;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(EditAnywhere, Category="GAS|Abilities|Effects")
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	UPROPERTY(EditAnywhere, Category="GAS|Abilities|Effects")
	TObjectPtr<USoundBase> ImpactSound;
	
	UPROPERTY(EditAnywhere, Category="GAS|Abilities|Effects")
	TObjectPtr<USoundBase> LoopingSound;

	UPROPERTY()
	TObjectPtr<UAudioComponent> LoopingSoundComponent;

public:	
};
