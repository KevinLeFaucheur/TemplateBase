// Retropsis @ 2024


#include "Inventory/HotbarComponent.h"
#include "Data/ItemInfo.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/KismetSystemLibrary.h"

bool UHotbarComponent::CheckHotbarAtIndex(int32 Index, EItemType& ItemType)
{
	if(!Items.IsValidIndex(Index)) return false;

	if(Items[Index].ID == 0)
	{
		return false;
	}
	
	if(const UItemInfo* ItemInfo = Cast<UItemInfo>(UKismetSystemLibrary::LoadAsset_Blocking(Items[Index].Asset.Get())))
	{
		ItemType = ItemInfo->ItemData.ItemType;
		return true;
	}
	return false;
}

void UHotbarComponent::HandleOnSlotDrop(UInventoryComponent* SourceInventory, int32 SourceSlotIndex,  int32 TargetSlotIndex)
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

bool UHotbarComponent::AddItemToIndex(FInventoryItemData InventoryItemData, int32 SourceSlotIndex, int32 TargetSlotIndex)
{
	const bool bWasSuccessful = Super::AddItemToIndex(InventoryItemData, SourceSlotIndex, TargetSlotIndex);

	IPlayerInterface::Execute_UpdateInventorySlot(GetOwner(), ContainerType, TargetSlotIndex, GetItemAtIndex(TargetSlotIndex));
	
	return bWasSuccessful;
}

bool UHotbarComponent::RemoveItemAtIndex(int32 SlotIndex)
{
	const bool bWasSuccessful = Super::RemoveItemAtIndex(SlotIndex);

	IPlayerInterface::Execute_ResetInventorySlot(GetOwner(), ContainerType, SlotIndex);

	return bWasSuccessful;
}
