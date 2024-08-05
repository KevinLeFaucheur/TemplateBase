// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "Harvesting/Harvestable.h"
#include "Interaction/GroundHarvestableInterface.h"
#include "GroundHarvestable.generated.h"

/**
 * 
 */
UCLASS()
class TEMPLATEBASE_API AGroundHarvestable : public AHarvestable, public IGroundHarvestableInterface
{
	GENERATED_BODY()

public:
	AGroundHarvestable();
	
	//~ Harvestable Interface
	virtual float GetHealth_Implementation() override { return Health; }
	virtual void UpdateHealth_Implementation(const float NewHealth) override { Health = NewHealth; }
	virtual TSoftObjectPtr<UHarvestableInfo> GetResources_Implementation() override { return Resources; }
	//~ Harvestable Interface

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;
	
};
