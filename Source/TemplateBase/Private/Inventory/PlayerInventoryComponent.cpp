// Retropsis @ 2024


#include "Inventory/PlayerInventoryComponent.h"

#include "Interaction/PlayerInterface.h"

void UPlayerInventoryComponent::HandleOnSlotDrop(UInventoryComponent* SourceInventory, int32 SourceSlotIndex,
                                                 int32 TargetSlotIndex)
{
	if(SourceInventory == this && SourceSlotIndex == TargetSlotIndex) return;

	switch (SourceInventory->ContainerType)
	{
	case EContainerType::Inventory:
	case EContainerType::Hotbar:
	case EContainerType::Storage:
		SourceInventory->TransferItem(this, SourceSlotIndex, TargetSlotIndex);
		break;
	case EContainerType::Armor:
		break;
	default: ;
	}
}

bool UPlayerInventoryComponent::AddItemToIndex(FInventoryItemData InventoryItemData, int32 SourceSlotIndex,
	int32 TargetSlotIndex)
{
	const bool bWasSuccessful = Super::AddItemToIndex(InventoryItemData, SourceSlotIndex, TargetSlotIndex);

	IPlayerInterface::Execute_UpdateInventorySlot(GetOwner(), ContainerType, TargetSlotIndex, GetItemAtIndex(TargetSlotIndex));
	
	return bWasSuccessful;
}

bool UPlayerInventoryComponent::RemoveItemAtIndex(int32 SlotIndex)
{
	const bool bWasSuccessful = Super::RemoveItemAtIndex(SlotIndex);

	IPlayerInterface::Execute_ResetInventorySlot(GetOwner(), ContainerType, SlotIndex);

	return bWasSuccessful;
}
