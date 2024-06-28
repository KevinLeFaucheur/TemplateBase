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

void UInventoryComponent::ServerAddItem_Implementation(FInventoryItemData ItemToAdd)
{
	AddItem(ItemToAdd);
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

void UInventoryComponent::AddItem(FInventoryItemData ItemToAdd)
{
	int32 EmptySlotIndex;
	if(FindEmptySlot(EmptySlotIndex))
	{
		Items.EmplaceAt(EmptySlotIndex, ItemToAdd);
		UpdateInventorySlotUI(EmptySlotIndex, ItemToAdd);
	}
}

void UInventoryComponent::UpdateInventorySlotUI(int32 SlotIndex, const FInventoryItemData& ItemData)
{
	switch (ContainerType)
	{
	case EContainerType::Inventory:
		IPlayerInterface::Execute_UpdateInventorySlot(GetOwner(), ContainerType, SlotIndex, ItemData);
		break;
	case EContainerType::Hotbar:
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

