// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryComponent.h"
#include "UObject/Interface.h"
#include "ControllerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UControllerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TEMPLATEBASE_API IControllerInterface
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void CloseInventory();

	UFUNCTION(BlueprintNativeEvent)
	void UpdateInventorySlot(EContainerType ContainerType, int32 SlotIndex, FInventoryItemData ItemData);
};
