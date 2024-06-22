// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "Equipment/Tool.h"
#include "World/Pickup.h"
#include "AmmunitionPickup.generated.h"

/**
 * 
 */
UCLASS()
class TEMPLATEBASE_API AAmmunitionPickup : public APickup
{
	GENERATED_BODY()

protected:
	virtual void OnSphereBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;

	UPROPERTY(EditAnywhere)
	int32 AmmunitionAmount = 15;

	UPROPERTY(EditAnywhere)
	EToolType AmmunitionType = EToolType::ETT_Handgun;
};
