// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryComponent.h"
#include "PlayerInventoryComponent.generated.h"

/**
 * 
 */
UCLASS()
class TEMPLATEBASE_API UPlayerInventoryComponent : public UInventoryComponent
{
	GENERATED_BODY()

public:
	virtual void HandleOnSlotDrop(UInventoryComponent* SourceInventory, int32 SourceSlotIndex, int32 TargetSlotIndex) override;
	virtual bool AddItemToIndex(FInventoryItemData InventoryItemData, int32 SourceSlotIndex, int32 TargetSlotIndex) override;
	virtual bool RemoveItemAtIndex(int32 SlotIndex) override;
};
