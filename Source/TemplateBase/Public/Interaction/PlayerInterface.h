// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "Data/ArmorInfo.h"
#include "Inventory/InventoryComponent.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TEMPLATEBASE_API IPlayerInterface
{
	GENERATED_BODY()

public:
	virtual void PlayHitReactMontage() {}
	
	UFUNCTION(BlueprintNativeEvent)
	int32 GetXP() const;
	
	UFUNCTION(BlueprintNativeEvent)
	int32 FindLevelForXP(int32 InXP) const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePointReward(int32 Level) const;
	
	UFUNCTION(BlueprintNativeEvent)
	int32 GetSpellPointReward(int32 Level) const;
	
	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePoints() const;
	
	UFUNCTION(BlueprintNativeEvent)
	int32 GetSpellPoints() const;

	UFUNCTION(BlueprintNativeEvent)
	void AddToXP(int32 InXP);
	
	UFUNCTION(BlueprintNativeEvent)
	void AddToPlayerLevel(int32 InPlayerLevel);

	UFUNCTION(BlueprintNativeEvent)
	void AddToAttributePoints(int32 InAttributePoints);
	
	UFUNCTION(BlueprintNativeEvent)
	void AddToSpellPoints(int32 InSpellPoints);
	
	UFUNCTION(BlueprintNativeEvent)
	void LevelUp();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ToggleMagicCircle(bool bShow, UMaterialInterface* DecalMaterial = nullptr);
	
	UFUNCTION(BlueprintNativeEvent)
	void PickupAmmunition(EToolType ToolType, int32 AmmunitionAmount);

	UFUNCTION(BlueprintNativeEvent)
	void UpdateInventorySlot(EContainerType ContainerType, int32 SlotIndex, FInventoryItemData ItemData);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnSlotDrop(EContainerType TargetContainer, EContainerType SourceContainer, int32 SourceSlotIndex, int32 TargetSlotIndex, EArmorType ArmorType);

	UFUNCTION(BlueprintNativeEvent)
	void ResetInventorySlot(EContainerType ContainerType, int32 SlotIndex);
};
