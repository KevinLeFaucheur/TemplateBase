// Retropsis @ 2024

#include "AbilitySystem/Ability/DamageGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1);
	// for (TTuple<FGameplayTag, FDamageRange> Pair : DamageTypes)
	// {
	// 	const float ScaledMagnitudeMin = Pair.Value.DamageMin.GetValueAtLevel(GetAbilityLevel());
	// 	const float ScaledMagnitudeMax = Pair.Value.DamageMax.GetValueAtLevel(GetAbilityLevel());
	// 	const float Magnitude = FMath::FRandRange(ScaledMagnitudeMin, ScaledMagnitudeMax);
	// 	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, Pair.Key, Magnitude);
	// }
	const float ScaledMagnitudeMin = DamageRange.DamageMin.GetValueAtLevel(GetAbilityLevel());
	const float ScaledMagnitudeMax = DamageRange.DamageMax.GetValueAtLevel(GetAbilityLevel());
	const float Magnitude = FMath::FRandRange(ScaledMagnitudeMin, ScaledMagnitudeMax);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DamageType, Magnitude);
	
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(
		*DamageSpecHandle.Data.Get(),
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}

FTaggedMontage UDamageGameplayAbility::GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const
{
	if (TaggedMontages.Num() > 0)
	{
		return TaggedMontages[FMath::RandRange(0, TaggedMontages.Num() - 1)];
	}
	return FTaggedMontage();
}

float UDamageGameplayAbility::GetMinDamageByDamageType(float InLevel, const FGameplayTag& InDamageType)
{
	// checkf(DamageTypes.Contains(InDamageType), TEXT("Gameplay Ability [%s] does not contain Damage Type [%s]"), *GetNameSafe(this), *DamageType.ToString());
	// return DamageTypes[InDamageType].DamageMin.GetValueAtLevel(InLevel);
	return DamageRange.DamageMin.GetValueAtLevel(InLevel);
}

float UDamageGameplayAbility::GetMaxDamageByDamageType(float InLevel, const FGameplayTag& InDamageType)
{
	// checkf(DamageTypes.Contains(InDamageType), TEXT("Gameplay Ability [%s] does not contain Damage Type [%s]"), *GetNameSafe(this), *DamageType.ToString());
	// return DamageTypes[InDamageType].DamageMax.GetValueAtLevel(InLevel);
	return DamageRange.DamageMax.GetValueAtLevel(InLevel);
}