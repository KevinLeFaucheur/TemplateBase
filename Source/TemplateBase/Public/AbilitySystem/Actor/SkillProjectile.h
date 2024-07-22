// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Actor/SpellProjectile.h"
#include "SkillProjectile.generated.h"

class UNiagaraComponent;
/**
 * 
 */
UCLASS()
class TEMPLATEBASE_API ASkillProjectile : public ASpellProjectile
{
	GENERATED_BODY()

public:
	ASkillProjectile();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GAS|Abilities|Effects")
	TObjectPtr<UNiagaraComponent> ProjectileEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GAS|Abilities|Effects", meta=(ExposeOnSpawn))
	TObjectPtr<UNiagaraSystem> MuzzleEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GAS|Abilities|Effects", meta=(ExposeOnSpawn))
	FVector EffectSpawnOffset;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GAS|Abilities|Effects", meta=(ExposeOnSpawn))
	float ProjectileDestroyDelay = 0.f;

protected:
	virtual void BeginPlay() override;
	virtual void OnSphereBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent*OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;

private:
	void SpawnVFX(UNiagaraSystem* EffectToSpawn, const FVector& SpawnOffset) const;
};
