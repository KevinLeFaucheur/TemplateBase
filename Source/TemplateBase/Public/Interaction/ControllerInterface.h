// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryComponent.h"
#include "UI/Data/UserWidgetData.h"
#include "UObject/Interface.h"
#include "ControllerInterface.generated.h"

enum class EArmorType : uint8;

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

	UFUNCTION(BlueprintNativeEvent)
	void ResetInventorySlot(EContainerType ContainerType, int32 SlotIndex);
	
	UFUNCTION(BlueprintNativeEvent)
	void ShowNotification(UTexture2D* Icon, int32 Quantity, const FText& NotificationText, ENotificationType NotificationType);
};
