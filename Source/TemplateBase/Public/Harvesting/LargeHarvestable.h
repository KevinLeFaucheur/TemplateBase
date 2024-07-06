// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "Harvesting/Harvestable.h"
#include "LargeHarvestable.generated.h"

/**
 * 
 */
UCLASS()
class TEMPLATEBASE_API ALargeHarvestable : public AHarvestable
{
	GENERATED_BODY()

public:
	ALargeHarvestable();
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;
};
