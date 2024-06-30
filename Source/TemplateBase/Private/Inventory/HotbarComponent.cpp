// Retropsis @ 2024


#include "Inventory/HotbarComponent.h"
#include "Data/ItemInfo.h"
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
