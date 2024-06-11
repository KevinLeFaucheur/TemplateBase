// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "BaseGameplayTags.h"
#include "GameplayTagContainer.h"
#include "UI/Controller/BaseWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FSpellSlotSelectedSignature, bool, bSpendPointButtonEnabled, bool, bEquipSlotButtonEnabled, FString, Description, FString, NextLevelDescription);

struct FSelectedAbility
{
	FGameplayTag Ability = FGameplayTag();
	FGameplayTag Status = FGameplayTag();
};

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class TEMPLATEBASE_API USpellMenuWidgetController : public UBaseWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UFUNCTION(BlueprintCallable)
	void SpellSlotSelected(const FGameplayTag& AbilityTag);

	UFUNCTION(BlueprintCallable)
	void SpendPointButtonPressed();

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStatChangedSignature SpellPointsChanged;

	UPROPERTY(BlueprintAssignable)
	FSpellSlotSelectedSignature OnSpellSlotSelected;

private:
	static void ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 SpellPoints, bool& bShouldEnableSpendPointButton, bool& bShouldEnableEquipButton);
	FSelectedAbility SelectedAbility = { FBaseGameplayTags::Get().Abilities_None, FBaseGameplayTags::Get().Abilities_Status_Locked };
	int32 CurrentSpellPoints = 0;
};
