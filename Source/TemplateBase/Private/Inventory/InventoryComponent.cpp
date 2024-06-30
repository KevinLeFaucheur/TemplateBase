// Retropsis @ 2024

#include "Inventory/InventoryComponent.h"
#include "Interaction/PlayerInterface.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();	
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UInventoryComponent::FindEmptySlot(int32& EmptySlotIndex)
{
	for (int i = 0; i < Items.Num(); ++i)
	{
		if (Items[i].ID == 0)
		{
			EmptySlotIndex = i;
			return true;
		}
	}
	return false;
}

void UInventoryComponent::ServerAddItem_Implementation(FInventoryItemData ItemToAdd)
{
	AddItem(ItemToAdd);
}

void UInventoryComponent::AddItem(const FInventoryItemData& ItemToAdd)
{
	int32 EmptySlotIndex;
	if(FindEmptySlot(EmptySlotIndex))
	{
		Items[EmptySlotIndex] = ItemToAdd;
		UpdateInventorySlotUI(EmptySlotIndex, ItemToAdd);
	}
}

bool UInventoryComponent::AddItemToIndex(FInventoryItemData InventoryItemData, int32 SourceSlotIndex, int32 TargetSlotIndex)
{
	if(IsSlotEmpty(TargetSlotIndex))
	{
		Items[TargetSlotIndex] = InventoryItemData;
		return true;
	}
	else
	{
		// Stacking Items
	}
	return false;
}

bool UInventoryComponent::RemoveItemAtIndex(int32 SlotIndex)
{
	Items[SlotIndex] = FInventoryItemData();
	return true;
}

void UInventoryComponent::OnSlotDrop_Implementation(UInventoryComponent* SourceInventory, int32 SourceSlotIndex, int32 TargetSlotIndex)
{
	HandleOnSlotDrop(SourceInventory, SourceSlotIndex, TargetSlotIndex);
}

void UInventoryComponent::HandleOnSlotDrop(UInventoryComponent* SourceInventory, int32 SourceSlotIndex, int32 TargetSlotIndex) {}

void UInventoryComponent::TransferItem(UInventoryComponent* TargetInventory, int32 SourceSlotIndex, int32 TargetSlotIndex)
{
	if(IsSlotEmpty(SourceSlotIndex) || (SourceSlotIndex == TargetSlotIndex && TargetInventory == this)) return;

	if(TargetInventory)
	{
		const FInventoryItemData ItemData = GetItemAtIndex(SourceSlotIndex);
		if (TargetInventory->AddItemToIndex(ItemData, SourceSlotIndex, TargetSlotIndex))
		{
			RemoveItemAtIndex(SourceSlotIndex);
		}
	}
}

bool UInventoryComponent::IsSlotEmpty(int32 SlotIndex)
{
	checkf(Items.IsValidIndex(SlotIndex), TEXT("Attempting to reach out of bounds index in %s"), *GetNameSafe(this));
	return Items[SlotIndex].ID == 0;
}

FInventoryItemData UInventoryComponent::GetItemAtIndex(int32 SlotIndex)
{
	return Items.IsValidIndex(SlotIndex) ? Items[SlotIndex] : FInventoryItemData();
}

void UInventoryComponent::UpdateInventorySlotUI(int32 SlotIndex, const FInventoryItemData& ItemData)
{
	// TODO: Remainder if custom behaviour is needed, don't forget to break
	switch (ContainerType)
	{
	case EContainerType::Inventory:
	case EContainerType::Hotbar:
		IPlayerInterface::Execute_UpdateInventorySlot(GetOwner(), ContainerType, SlotIndex, ItemData);
		break;
	case EContainerType::Storage:
		break;
	case EContainerType::Armor:
		break;
	default: ;
	}
}

void UInventoryComponent::SetInventorySize(const int32 Size)
{
	Items.SetNum(Size, EAllowShrinking::No);
}

