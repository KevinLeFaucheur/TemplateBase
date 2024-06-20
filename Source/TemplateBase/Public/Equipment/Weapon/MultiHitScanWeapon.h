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

private:
	UPROPERTY(EditAnywhere, Category="Equipment|MultiHitScan")
	int32 NumHits = 12;
};
