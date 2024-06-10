// Retropsis @ 2024

#include "UI/Controller/BaseWidgetController.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/PlayerCharacterController.h"
#include "Player/PlayerCharacterState.h"

void UBaseWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UBaseWidgetController::BroadcastAbilityInfo()
{
	if (!GetBaseAbilitySystemComponent()->bStartupAbilitiesGiven) return;

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda(
		 [this] (const FGameplayAbilitySpec& AbilitySpec)
		{
			FBaseAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(BaseAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
			 Info.InputTag = BaseAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
			 AbilityInfoDelegate.Broadcast(Info);
		}
	);
	GetBaseAbilitySystemComponent()->ForEachAbility(BroadcastDelegate);
}

APlayerCharacterController* UBaseWidgetController::GetPlayerCharacterController()
{
	if(PlayerCharacterController == nullptr)
	{
		PlayerCharacterController = Cast<APlayerCharacterController>(PlayerController);
	}
	return PlayerCharacterController;
}

APlayerCharacterState* UBaseWidgetController::GetPlayerCharacterState()
{
	if(PlayerCharacterState == nullptr)
	{
		PlayerCharacterState = Cast<APlayerCharacterState>(PlayerState);
	}
	return PlayerCharacterState;
}

UBaseAbilitySystemComponent* UBaseWidgetController::GetBaseAbilitySystemComponent()
{
	if(BaseAbilitySystemComponent == nullptr)
	{
		BaseAbilitySystemComponent = Cast<UBaseAbilitySystemComponent>(AbilitySystemComponent);
	}
	return BaseAbilitySystemComponent;
}

UBaseAttributeSet* UBaseWidgetController::GetBaseAttributeSet()
{
	if(BaseAttributeSet == nullptr)
	{
		BaseAttributeSet = Cast<UBaseAttributeSet>(AttributeSet);
	}
	return BaseAttributeSet;
}
