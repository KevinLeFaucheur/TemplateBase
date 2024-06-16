// Retropsis @ 2024

#include "AbilitySystem/Ability/PassiveAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"

void UPassiveAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                      const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                      const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if(UBaseAbilitySystemComponent* BaseASC = Cast<UBaseAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo())))
	{
		BaseASC->DeactivatePassiveAbility.AddUObject(this, &UPassiveAbility::ReceiveDeactivate);
	}	
}

void UPassiveAbility::ReceiveDeactivate(const FGameplayTag& AbilityTag)
{
	if(AbilityTags.HasTagExact(AbilityTag))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}
