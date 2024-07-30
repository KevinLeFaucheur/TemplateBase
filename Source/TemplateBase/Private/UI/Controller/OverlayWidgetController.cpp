// Retropsis @ 2024

#include "UI/Controller/OverlayWidgetController.h"

#include "BaseGameplayTags.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/AlterationEffectInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/PlayerCharacterController.h"
#include "Player/PlayerCharacterState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	OnHealthChanged.Broadcast(GetBaseAttributeSet()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetBaseAttributeSet()->GetMaxHealth());
	OnManaChanged.Broadcast(GetBaseAttributeSet()->GetMana());
	OnMaxManaChanged.Broadcast(GetBaseAttributeSet()->GetMaxMana());
	OnPlayerNameChanged.Broadcast(GetPlayerCharacterState()->GetPlayerName());
	// OnXPValueChanged.Broadcast(GetPlayerCharacterState()->GetXP());
	OnLevelRequirementChanged(GetPlayerCharacterState()->GetPlayerLevel());
	OnXPChanged(GetPlayerCharacterState()->GetXP());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	GetPlayerCharacterState()->OnXPChanged.AddUObject(this, &UOverlayWidgetController::OnXPChanged);
	GetPlayerCharacterState()->OnLevelChanged.AddLambda(
		 [this] (int32 NewLevel)
		{
			OnPlayerLevelChanged.Broadcast(NewLevel);
		 	OnLevelRequirementChanged(NewLevel);
		}
	);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetBaseAttributeSet()->GetHealthAttribute()).AddLambda(
		[this] (const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetBaseAttributeSet()->GetMaxHealthAttribute()).AddLambda(
		[this] (const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetBaseAttributeSet()->GetManaAttribute()).AddLambda(
		[this] (const FOnAttributeChangeData& Data)
		{
			OnManaChanged.Broadcast(Data.NewValue);
		}
	);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetBaseAttributeSet()->GetMaxManaAttribute()).AddLambda(
		[this] (const FOnAttributeChangeData& Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		}
	);
	for (FGameplayTag AlterationEffectTag : FBaseGameplayTags::Get().AlterationEffects)
	{
		AbilitySystemComponent->RegisterGameplayTagEvent(AlterationEffectTag, EGameplayTagEventType::NewOrRemoved).AddLambda(
			[this, AlterationEffectTag] (const FGameplayTag Tag, int32 NewCount) {
				if(NewCount > 0)
				{
					FAlterationEffect AlterationEffect = AlterationEffectInfo->FindAlterationEffectForTag(AlterationEffectTag);
					
					TArray<FGameplayEffectSpec> OutSpecCopies;
					AbilitySystemComponent->GetAllActiveGameplayEffectSpecs(OutSpecCopies);
					for (FGameplayEffectSpec Spec : OutSpecCopies)
					{
						if(Spec.Def->GetGrantedTags().HasTagExact(AlterationEffectTag))
						{
							AlterationEffect.TimeRemaining = Spec.Duration;
							break;
						}
					}
					OnAlterationEffectAdded.Broadcast(AlterationEffect);
				}
				else
				{
					OnAlterationEffectRemoved.Broadcast(AlterationEffectTag);
				}
			});
	}
	
	if (GetBaseAbilitySystemComponent())
	{
		GetBaseAbilitySystemComponent()->AbilityEquipped.AddUObject(this, &UOverlayWidgetController::OnAbilityEquipped);
		GetBaseAbilitySystemComponent()->ToolAbilityEquipped.AddUObject(this, &UOverlayWidgetController::OnToolAbilityEquipped);
		if(GetBaseAbilitySystemComponent()->bStartupAbilitiesGiven)
		{
			BroadcastAbilityInfo();
		}
		else
		{
			GetBaseAbilitySystemComponent()->AbilitiesGiven.AddUObject(this, &UOverlayWidgetController::BroadcastAbilityInfo);
		}
	
		GetBaseAbilitySystemComponent()->EffectAssetTags.AddLambda(
			[this] (const FGameplayTagContainer& AssetTags)
			{
				for (const FGameplayTag& Tag : AssetTags)
				{
					//  "Message.HealthPotion".MatchesTag("Message") will return True, "Message".MatchesTag("Message.HealthPotion") will return False
					FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
					if (Tag.MatchesTag(MessageTag))
					{
						const FUIWidgetRow* Row =  GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
						MessageWidgetRow.Broadcast(*Row);
					}
				}
			}
		);
	}
	if(APlayerCharacterController* CharacterPlayerController = Cast<APlayerCharacterController>(PlayerController))
	{
		CharacterPlayerController->AmmunitionChanged.AddLambda([this](int32 NewCount)
		{
			OnAmmunitionChanged.Broadcast(NewCount);
		});
		CharacterPlayerController->CarriedAmmunitionChanged.AddLambda([this](int32 NewCount)
		{
			OnCarriedAmmunitionChanged.Broadcast(NewCount);
		});
		CharacterPlayerController->ThrowableCountChanged.AddLambda([this](int32 NewCount)
		{
			OnGrenadeCountChanged.Broadcast(NewCount);
		});
	}
}

void UOverlayWidgetController::OnXPChanged(int32 NewXP)
{
	const UPlayerLevelUpInfo* LevelUpInfo = GetPlayerCharacterState()->LevelUpInfo;
	checkf(LevelUpInfo, TEXT("Unable to find LevelUpInfon please fill out Player State"));

	const int32 Level = LevelUpInfo->FindLevelForXP(NewXP);
	const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num();

	if(Level <= MaxLevel && Level > 0)
	{
		const int32 LevelUpRequirement = LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
		const int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInformation[Level - 1].LevelUpRequirement;

		const int32 DeltaLevelUpRequirement = LevelUpRequirement - PreviousLevelUpRequirement;
		const int32 XPForThisLevel = NewXP - PreviousLevelUpRequirement;

		const float XPBarPercent = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLevelUpRequirement);
		
		OnXPPercentChanged.Broadcast(XPBarPercent);
		// OnXPValueChanged.Broadcast(NewXP);
		OnXPValueChanged.Broadcast(XPForThisLevel);
	}
}

void UOverlayWidgetController::OnLevelRequirementChanged(int32 NewLevel)
{
	const UPlayerLevelUpInfo* LevelUpInfo = GetPlayerCharacterState()->LevelUpInfo;
	checkf(LevelUpInfo, TEXT("Unable to find LevelUpInfon please fill out Player State"));

	const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num();

	if(NewLevel <= MaxLevel && NewLevel > 0)
	{
		const int32 LevelUpRequirement = LevelUpInfo->LevelUpInformation[NewLevel].LevelUpRequirement;
		const int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInformation[NewLevel - 1].LevelUpRequirement;
		const int32 DeltaLevelUpRequirement = LevelUpRequirement - PreviousLevelUpRequirement;
		OnLevelRequirementValueChanged.Broadcast(DeltaLevelUpRequirement);
	}
}

void UOverlayWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot) const
{
	const FBaseGameplayTags GameplayTags = FBaseGameplayTags::Get();
	FBaseAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag = GameplayTags.Abilities_Status_Unlocked;
	LastSlotInfo.InputTag = PreviousSlot;
	LastSlotInfo.AbilityTag = GameplayTags.Abilities_None;

	// Broadcast empty slot info if Previous slot is valid slot, for equipping an already equipped spell
	AbilityInfoDelegate.Broadcast(LastSlotInfo);
	
	FBaseAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = Status;
	Info.InputTag = Slot;
	AbilityInfoDelegate.Broadcast(Info);
}

void UOverlayWidgetController::OnToolAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status,
	const FGameplayTag& Slot, const FGameplayTag& PreviousSlot) const
{
	FBaseAbilityInfo Info = ToolAbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = Status;
	Info.InputTag = Slot;
	Info.AbilityTag = AbilityTag;
	AbilityInfoDelegate.Broadcast(Info);
}
