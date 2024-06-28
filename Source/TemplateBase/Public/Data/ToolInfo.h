// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "Data/ItemInfo.h"
#include "ToolInfo.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FToolData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Tool")
	TSoftClassPtr<ATool> ToolClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Tool")
	float BaseDamage = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Tool")
	bool bUseAmmunition = true;

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
