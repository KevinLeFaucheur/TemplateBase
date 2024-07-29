// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Data/AlterationEffectInfo.h"
#include "UI/Controller/BaseWidgetController.h"
#include "OverlayWidgetController.generated.h"

struct FBaseAbilityInfo;
class UBaseAbilitySystemComponent;
class UAbilityInfo;
class UBaseUserWidget;

struct FOnAttributeChangeData;

USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag AssetTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UBaseUserWidget> MessageWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Image = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FUIWidgetRow, Row);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAmmunitionChanged, int32, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnXPValueChanged, int32, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerNameChanged, FString, PlayerName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAlterationEffectAdded, FAlterationEffect, AlterationEffect);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAlterationEffectRemoved, const FGameplayTag&, AlterationEffectTag);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class TEMPLATEBASE_API UOverlayWidgetController : public UBaseWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
	
	 UFUNCTION()
	void OnXPChanged(int32 NewXP);

	 UFUNCTION()
	void OnLevelRequirementChanged(int32 NewXP);

	UPROPERTY(BlueprintAssignable, Category="PlayerCharacter")
	FOnPlayerNameChanged OnPlayerNameChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnManaChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Messages")
	FMessageWidgetRowSignature MessageWidgetRow;

	UPROPERTY(BlueprintAssignable, Category="GAS|Messages")
	FOnAlterationEffectAdded OnAlterationEffectAdded;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Messages")
	FOnAlterationEffectRemoved OnAlterationEffectRemoved;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Meta")
	FOnAttributeChangedSignature OnXPPercentChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Meta")
	FOnXPValueChanged OnXPValueChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Meta")
	FOnXPValueChanged OnLevelRequirementValueChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Meta")
	FOnPlayerStatChangedSignature OnPlayerLevelChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Meta")
	FOnPlayerStatChangedSignature OnAttributePointsChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Meta")
	FOnPlayerStatChangedSignature OnSpellPointsChanged;

	// TODO: Maybe better solution
	UPROPERTY(BlueprintAssignable, Category="Equipment")
	FOnAmmunitionChanged OnAmmunitionChanged;
	
	UPROPERTY(BlueprintAssignable, Category="Equipment")
	FOnAmmunitionChanged OnCarriedAmmunitionChanged;
	
	UPROPERTY(BlueprintAssignable, Category="Equipment")
	FOnAmmunitionChanged OnGrenadeCountChanged;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GAS|WidgetData")
	TObjectPtr<UDataTable> MessageWidgetDataTable;

	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);

	void OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot) const;
	void OnToolAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot) const;
};

template <typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
}
