// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "Data/ItemInfo.h"
#include "ToolInfo.generated.h"

class AItemBase;

UENUM()
enum class EToolClass : uint8
{
	Tool UMETA(DisplayName="Default Tool"),
	HarvestingTool UMETA(DisplayName="Harvesting Tool"),
	MeleeWeapon UMETA(DisplayName="MeleeWeapon"),
	RangeWeapon UMETA(DisplayName="RangeWeapon")
};

USTRUCT(Blueprintable, BlueprintType)
struct FToolData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Tool")
	TSoftClassPtr<AItemBase> ToolClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Tool")
	float BaseDamage = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Tool")
	bool bUseAmmunition = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Tool")
	int32 Ammunition = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Tool")
	int32 MaxAmmunition = 0;
};

/**
 * 
 */
UCLASS()
class TEMPLATEBASE_API UToolInfo : public UItemInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Tool")
	FToolData ToolData;
};
