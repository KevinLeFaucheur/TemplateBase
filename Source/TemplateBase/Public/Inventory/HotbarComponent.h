// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryComponent.h"
#include "HotbarComponent.generated.h"

enum class EItemType : uint8;

/**
 * 
 */
UCLASS()
class TEMPLATEBASE_API UHotbarComponent : public UInventoryComponent
{
	GENERATED_BODY()

public:
	bool CheckHotbarAtIndex(int32 Index, EItemType& ItemType);
	virtual void HandleOnSlotDrop(UInventoryComponent* SourceInventory, int32 SourceSlotIndex, int32 TargetSlotIndex) override;
	virtual bool AddItemToIndex(FInventoryItemData InventoryItemData, int32 SourceSlotIndex, int32 TargetSlotIndex) override;
	virtual bool RemoveItemAtIndex(int32 SlotIndex) override;
};
