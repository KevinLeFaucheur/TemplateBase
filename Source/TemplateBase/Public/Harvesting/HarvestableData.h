#pragma once

#include "CoreMinimal.h"
#include "HarvestableData.generated.h"

class UItemInfo;

UENUM(BlueprintType)
enum class EHarvestingToolType : uint8
{
	Hatchet UMETA(DisplayName="Hatchet"),
	Pickaxe UMETA(DisplayName="Pickaxe"),
};
