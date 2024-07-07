// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "Equipment/Weapon/HitScanWeapon.h"
#include "MultiHitScanWeapon.generated.h"

/**
 * 
 */
UCLASS()
class TEMPLATEBASE_API AMultiHitScanWeapon : public AHitScanWeapon
{
	GENERATED_BODY()

public:
	virtual void Activate(const FVector& HitTarget) override;
	virtual void ActivateHitStack(const FVector& HitTarget, FVector Start, AActor* OwnerPawn);
	virtual void ActivatePerHit(const FVector& HitTarget, FVector Start, AActor* OwnerPawn);

private:
	UPROPERTY(EditAnywhere, Category="Equipment|MultiHitScan")
	bool bShouldStack = true;
	
	UPROPERTY(EditAnywhere, Category="Equipment|MultiHitScan")
	float MaxDelay = .5f;
	
	UPROPERTY(EditAnywhere, Category="Equipment|MultiHitScan")
	int32 NumHits = 12;
};
