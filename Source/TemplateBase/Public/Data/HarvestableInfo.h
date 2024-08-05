// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Harvesting/HarvestableData.h"
#include "HarvestableInfo.generated.h"

/**
 * 
*/

class ATransientHarvestable;

USTRUCT(BlueprintType)
struct FResource
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UItemInfo> Resource = nullptr;

	UPROPERTY(EditAnywhere)
	int32 Quantity = 0;

	UPROPERTY(EditAnywhere)
	EHarvestingToolType PreferredTool = EHarvestingToolType::Hatchet;
};

UCLASS(BlueprintType, Blueprintable)
class TEMPLATEBASE_API UHarvestableInfo : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FResource> GivenResources;

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<ATransientHarvestable> TransientHarvestableClass;
};
