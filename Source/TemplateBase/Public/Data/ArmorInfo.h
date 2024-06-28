// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "Data/ItemInfo.h"
#include "ArmorInfo.generated.h"

UENUM(BlueprintType)
enum class EArmorType : uint8
{
	Head UMETA(DisplayName="Head"),
	UpperBody UMETA(DisplayName="UpperBody"),
	LowerBody UMETA(DisplayName="LowerBody"),
	Feet UMETA(DisplayName="Feet"),
	Wrist UMETA(DisplayName="Wrist"),
	Waist UMETA(DisplayName="Waist"),
	Neck UMETA(DisplayName="Neck")
};

USTRUCT(BlueprintType)
struct FArmorData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Armor")
	EArmorType ArmorType = EArmorType::Head;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Armor")
	float BaseArmor = 0.f;

	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	// TSoftClassPtr<ATool> ToolClass;
	
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class TEMPLATEBASE_API UArmorInfo : public UItemInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Armor")
	FArmorData ArmorData;
};
