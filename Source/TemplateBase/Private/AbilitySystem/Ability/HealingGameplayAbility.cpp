// Retropsis @ 2024

#include "AbilitySystem/Ability/HealingGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UHealingGameplayAbility::SingleTargetHeal(AActor* TargetActor) const
{
	const FGameplayEffectSpecHandle HealingSpecHandle = MakeOutgoingGameplayEffectSpec(HealingEffectClass, 1);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(HealingSpecHandle, HealingType, GetHealingPowerAtLevel());
	
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(
		*HealingSpecHandle.Data.Get(),
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}

float UHealingGameplayAbility::GetHealingPowerAtLevel() const
{
	return FMath::FRandRange(GetMinHealingByLevel(GetAbilityLevel()), GetMaxHealingByLevel(GetAbilityLevel()));
}

float UHealingGameplayAbility::GetMinHealingByLevel(float InLevel) const
{
	return HealingRange.DamageMin.GetValueAtLevel(InLevel);
}

float UHealingGameplayAbility::GetMaxHealingByLevel(float InLevel) const
{
	return HealingRange.DamageMax.GetValueAtLevel(InLevel);
}
