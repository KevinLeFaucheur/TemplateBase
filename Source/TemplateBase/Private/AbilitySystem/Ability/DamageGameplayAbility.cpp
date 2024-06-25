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

FDamageEffectParams UDamageGameplayAbility::MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor, FVector InRadialDamageOrigin,
		bool bOverrideAirborneDirection, FVector AirborneDirectionOverride, bool bOverrideDeathImpulseDirection, FVector DeathImpulseDirectionOverride, bool bOverridePitch, float PitchOverride) const
{
	FDamageEffectParams Params;
	Params.WorldContextObject = GetAvatarActorFromActorInfo();
	Params.DamageEffectClass = DamageEffectClass;
	Params.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	Params.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	Params.BaseDamageRange.DamageMin = DamageRange.DamageMin.GetValueAtLevel(GetAbilityLevel());
	Params.BaseDamageRange.DamageMax = DamageRange.DamageMax.GetValueAtLevel(GetAbilityLevel());
	Params.AbilityLevel = GetAbilityLevel();
	Params.DamageType = DamageType;
	Params.StatusEffectChance = StatusEffectChance;
	Params.StatusEffectDamage = StatusEffectDamage;
	Params.StatusEffectDuration = StatusEffectDuration;
	Params.StatusEffectFrequency = StatusEffectFrequency;
	Params.DeathImpulseMagnitude = DeathImpulseMagnitude;
	Params.AirborneForceMagnitude = AirborneForceMagnitude;
	Params.AirborneChance = AirborneChance;

	if(IsValid(TargetActor))
	{
		FRotator Rotation = (TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).Rotation();
		if (bOverridePitch)
		{
			Rotation.Pitch = PitchOverride;
		}
		const FVector ToTarget = Rotation.Vector();
		if(!bOverrideAirborneDirection) Params.AirborneForce = ToTarget * AirborneForceMagnitude;  
		if(!bOverrideDeathImpulseDirection) Params.DeathImpulse = ToTarget * DeathImpulseMagnitude;
	}
	if(bOverrideAirborneDirection)
	{
		AirborneDirectionOverride.Normalize();
		Params.AirborneForce = AirborneDirectionOverride * AirborneForceMagnitude;
		if(bOverridePitch)
		{
			FRotator AirborneRotation = AirborneDirectionOverride.Rotation();
			AirborneRotation.Pitch = PitchOverride;
			Params.AirborneForce = AirborneRotation.Vector() * AirborneForceMagnitude;
		}
	}
	
	if(bOverrideDeathImpulseDirection)
	{
		DeathImpulseDirectionOverride.Normalize();
		Params.DeathImpulse = DeathImpulseDirectionOverride * DeathImpulseMagnitude;
		if(bOverridePitch)
		{
			FRotator DeathImpulseRotation = DeathImpulseDirectionOverride.Rotation();
			DeathImpulseRotation.Pitch = PitchOverride;
			Params.DeathImpulse = DeathImpulseRotation.Vector() * DeathImpulseMagnitude;
		}
	}

	if(bIsRadialDamage)
	{
		Params.bIsRadialDamage = bIsRadialDamage;
		Params.RadialDamageOrigin = InRadialDamageOrigin;
		Params.RadialDamageInnerRadius = RadialDamageInnerRadius;
		Params.RadialDamageOuterRadius = RadialDamageOuterRadius;
	}

	return Params;
}

float UDamageGameplayAbility::GetDamageAtLevel() const
{
	const float MinDamage = DamageRange.DamageMin.GetValueAtLevel(GetAbilityLevel());
	const float MaxDamage = DamageRange.DamageMax.GetValueAtLevel(GetAbilityLevel());
	return FMath::FRandRange(MinDamage, MaxDamage);
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