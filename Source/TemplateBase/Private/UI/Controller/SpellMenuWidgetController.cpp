// Retropsis @ 2024

#include "UI/Controller/SpellMenuWidgetController.h"

#include "BaseGameplayTags.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/PlayerCharacterState.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();
	SpellPointsChanged.Broadcast(GetPlayerCharacterState()->GetSpellPoints());
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	GetBaseAbilitySystemComponent()->AbilityStatusChanged.AddLambda(
		[this] (const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 NewLevel)
		{
			if(SelectedAbility.Ability.MatchesTagExact(AbilityTag))
			{
				SelectedAbility.Status = StatusTag;
				bool bEnableSpendPointButton = false;
				bool bEnableEquipButton = false;
				ShouldEnableButtons(StatusTag, CurrentSpellPoints, bEnableSpendPointButton, bEnableEquipButton);
				
				FString Description;
				FString NextLevelDescription;
				GetBaseAbilitySystemComponent()->GetDescriptionsByAbilityTag(AbilityTag, Description, NextLevelDescription);
				
				OnSpellSlotSelected.Broadcast(bEnableSpendPointButton, bEnableEquipButton, Description, NextLevelDescription);
			}
			
			if(AbilityInfo)
			{
				FBaseAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
				Info.StatusTag = StatusTag;
				AbilityInfoDelegate.Broadcast(Info);
			}
		}
	);
	GetPlayerCharacterState()->OnSpellPointsChanged.AddLambda(
		[this] (int32 SpellPoints)
		{
			SpellPointsChanged.Broadcast(SpellPoints);
			CurrentSpellPoints = SpellPoints;
			
			bool bEnableSpendPointButton = false;
			bool bEnableEquipButton = false;
			ShouldEnableButtons(SelectedAbility.Status, CurrentSpellPoints, bEnableSpendPointButton, bEnableEquipButton);
				
			FString Description;
			FString NextLevelDescription;
			GetBaseAbilitySystemComponent()->GetDescriptionsByAbilityTag(SelectedAbility.Ability, Description, NextLevelDescription);
			
			OnSpellSlotSelected.Broadcast(bEnableSpendPointButton, bEnableEquipButton, Description, NextLevelDescription);
		}
	);
}

void USpellMenuWidgetController::SpellSlotSelected(const FGameplayTag& AbilityTag)
{
	const FBaseGameplayTags GameplayTags = FBaseGameplayTags::Get();
	const int32 SpellPoints = GetPlayerCharacterState()->GetSpellPoints();
	FGameplayTag AbilityStatus;

	const bool bTagValid = AbilityTag.IsValid();
	const bool bTagNone = AbilityTag.MatchesTag(GameplayTags.Abilities_None);
	const FGameplayAbilitySpec* AbilitySpec = GetBaseAbilitySystemComponent()->GetSpecFromAbilityTag(AbilityTag);
	const bool bSpecValid = AbilitySpec != nullptr;
	if(!bTagValid || bTagNone || !bSpecValid)
	{
		AbilityStatus = GameplayTags.Abilities_Status_Locked;
	}
	else
	{
		AbilityStatus = GetBaseAbilitySystemComponent()->GetStatusTagFromSpec(*AbilitySpec);
	}

	SelectedAbility.Ability = AbilityTag;
	SelectedAbility.Status = AbilityStatus;
	bool bEnableSpendPointButton = false;
	bool bEnableEquipButton = false;
	ShouldEnableButtons(AbilityStatus, SpellPoints, bEnableSpendPointButton, bEnableEquipButton);
				
	FString Description;
	FString NextLevelDescription;
	GetBaseAbilitySystemComponent()->GetDescriptionsByAbilityTag(SelectedAbility.Ability, Description, NextLevelDescription);
	
	OnSpellSlotSelected.Broadcast(bEnableSpendPointButton, bEnableEquipButton, Description, NextLevelDescription);
}

void USpellMenuWidgetController::SpendPointButtonPressed()
{
	if(GetBaseAbilitySystemComponent()) GetBaseAbilitySystemComponent()->ServerSpendSpellPoint(SelectedAbility.Ability);
}

void USpellMenuWidgetController::ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 SpellPoints,
                                                     bool& bShouldEnableSpendPointButton, bool& bShouldEnableEquipButton)
{
	const FBaseGameplayTags GameplayTags = FBaseGameplayTags::Get();

	bShouldEnableEquipButton = false;
	bShouldEnableSpendPointButton = false;
	if(AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Equipped))
	{
		bShouldEnableEquipButton = true;
		bShouldEnableSpendPointButton = SpellPoints > 0;
	}
	else if(AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))
	{
		bShouldEnableSpendPointButton = SpellPoints > 0;
	}
	else if(AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
	{
		bShouldEnableEquipButton = true;
		bShouldEnableSpendPointButton = SpellPoints > 0;
	}
}
