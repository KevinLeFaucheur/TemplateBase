// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Actor/SpellProjectile.h"
#include "FireBall.generated.h"

/**
 * 
 */
UCLASS()
class TEMPLATEBASE_API AFireBall : public ASpellProjectile
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintImplementableEvent)
	void StartOutgoingTimeline();

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> ReturnToActor;
	
	UPROPERTY(BlueprintReadWrite)
	FDamageEffectParams RadialDamageEffectParams;
	
protected:
	virtual void BeginPlay() override;
	virtual void OnHit() override;
	
	virtual void OnSphereBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent*OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;

};
