// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemInfo.generated.h"

class ATool;

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Misc UMETA(DisplayName="Misc"),
	Resource UMETA(DisplayName="Resource"),
	Equipment UMETA(DisplayName="Equipment"),
	Armor UMETA(DisplayName="Armor"),
	Consumable UMETA(DisplayName="Consumable"),
	Buildable UMETA(DisplayName="Buildable")
};

USTRUCT(Blueprintable, BlueprintType)
struct FItemData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	int32 ID = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	TObjectPtr<UTexture2D> Icon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	FText Name = FText();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	FText Description = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	bool bIsStackable = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	int32 StackSize = 0;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	EItemType ItemType = EItemType::Misc;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	float Health = 0.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	float MaxHealth = 0.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	float Weight = 0.f;
};

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class TEMPLATEBASE_API UItemInfo : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Item")
	FItemData ItemData;
};
