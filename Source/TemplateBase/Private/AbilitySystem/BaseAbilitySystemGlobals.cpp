// Retropsis @ 2024

#include "AbilitySystem/BaseAbilitySystemGlobals.h"
#include "AbilitySystem/AbilityTypes.h"

FGameplayEffectContext* UBaseAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FBaseGameplayEffectContext();
}
