// Retropsis @ 2024

#include "AbilitySystem/ExecCalc/ExecCalc_MonkDamage.h"
#include "AbilitySystemComponent.h"
#include "BaseGameplayTags.h"
#include "AbilitySystem/AbilityTypes.h"
#include "AbilitySystem/BaseAbilitySystemLibrary.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"

struct BaseMonkDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Strength);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalAttack);
	
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);
	
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BluntResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CuttingResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PierceResistance);
	
	BaseMonkDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, Strength, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, PhysicalAttack, Source, false);
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, CriticalHitDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, CriticalHitResistance, Target, false);
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, PhysicalResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, BluntResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, CuttingResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, PierceResistance, Target, false);
	}
};

static const BaseMonkDamageStatics& MonkDamageStatics()
{
	static  BaseMonkDamageStatics MDStatics;
	return MDStatics;
}

UExecCalc_MonkDamage::UExecCalc_MonkDamage()
{
	RelevantAttributesToCapture.Add(MonkDamageStatics().StrengthDef);
	RelevantAttributesToCapture.Add(MonkDamageStatics().PhysicalAttackDef);
	
	RelevantAttributesToCapture.Add(MonkDamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(MonkDamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(MonkDamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(MonkDamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(MonkDamageStatics().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(MonkDamageStatics().CriticalHitResistanceDef);
	
	RelevantAttributesToCapture.Add(MonkDamageStatics().PhysicalResistanceDef);
	RelevantAttributesToCapture.Add(MonkDamageStatics().BluntResistanceDef);
	RelevantAttributesToCapture.Add(MonkDamageStatics().CuttingResistanceDef);
	RelevantAttributesToCapture.Add(MonkDamageStatics().PierceResistanceDef);
}

void UExecCalc_MonkDamage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;

	const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Primary_Strength, MonkDamageStatics().StrengthDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Secondary_PhysicalAttack, MonkDamageStatics().PhysicalAttackDef);
	
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Secondary_Armor, MonkDamageStatics().ArmorDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Secondary_ArmorPenetration, MonkDamageStatics().ArmorPenetrationDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Secondary_BlockChance, MonkDamageStatics().BlockChanceDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Secondary_CriticalHitChance, MonkDamageStatics().CriticalHitChanceDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Secondary_CriticalHitDamage, MonkDamageStatics().CriticalHitDamageDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Secondary_CriticalHitResistance, MonkDamageStatics().CriticalHitResistanceDef);
		
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Resistance_Physical, MonkDamageStatics().PhysicalResistanceDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Resistance_Blunt, MonkDamageStatics().BluntResistanceDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Resistance_Cutting, MonkDamageStatics().CuttingResistanceDef);
	TagsToCaptureDefs.Add(GameplayTags.Attributes_Resistance_Pierce, MonkDamageStatics().PierceResistanceDef);
	
	const UAbilitySystemComponent* SourceASC =  ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC =  ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	
	int32 SourceCharacterLevel = 1;
	if (SourceAvatar->Implements<UCombatInterface>())
	{
		SourceCharacterLevel = ICombatInterface::Execute_GetCharacterLevel(SourceAvatar);
	}
	int32 TargetCharacterLevel = 1;
	if (TargetAvatar->Implements<UCombatInterface>())
	{
		TargetCharacterLevel = ICombatInterface::Execute_GetCharacterLevel(TargetAvatar);
	}

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
	
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// Status Effects
	DetermineStatusEffect(ExecutionParams, Spec, EvaluationParameters, TagsToCaptureDefs);

	/*
	 * Get Damage Set By Caller Magnitude
	 */
	float Damage = 0.f;
	for (const TTuple<FGameplayTag, FGameplayTag>& Pair : FBaseGameplayTags::Get().DamageTypesToResistances)
	{
		const FGameplayTag ResistanceTag = Pair.Value;
		const FGameplayTag DamageTypeTag = Pair.Key;
		
		checkf(TagsToCaptureDefs.Contains(ResistanceTag), TEXT("TagsToCaptureDefs doesn't contain Tag [%s]: "), *ResistanceTag.ToString());
		
		const FGameplayEffectAttributeCaptureDefinition CaptureDef = TagsToCaptureDefs[ResistanceTag];

		float DamageTypeValue = Spec.GetSetByCallerMagnitude(Pair.Key, false);
		if(DamageTypeValue <= 0.f)
		{
			continue;
		}
		
		float Resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvaluationParameters, Resistance);
		Resistance = FMath::Clamp(Resistance, 0.f, 100.f);

		DamageTypeValue *= ( 100.f - Resistance ) / 100.f;

		if (UBaseAbilitySystemLibrary::IsRadialDamage(EffectContextHandle))
		{
			DamageTypeValue = UBaseAbilitySystemLibrary::GetRadialDamageWithFalloff(
			   TargetAvatar,
			   DamageTypeValue,
			   0.f,
			   UBaseAbilitySystemLibrary::GetRadialDamageOrigin(EffectContextHandle),
			   UBaseAbilitySystemLibrary::GetRadialDamageInnerRadius(EffectContextHandle),
			   UBaseAbilitySystemLibrary::GetRadialDamageOuterRadius(EffectContextHandle),
			   1.f);
		}
		
		// if(UBaseAbilitySystemLibrary::IsRadialDamage(EffectContextHandle))
		// {
		// 	if(ICombatInterface* CombatInterface = Cast<ICombatInterface>(TargetAvatar))
		// 	{
		// 		CombatInterface->GetOnTakeDamage().AddLambda([&] (float DamageAmount)
		// 		{
		// 			DamageTypeValue = DamageAmount;
		// 		});
		// 	}
		// 	UGameplayStatics::ApplyRadialDamageWithFalloff(
		// 		TargetAvatar,
		// 		DamageTypeValue,
		// 		0.f,
		// 		UBaseAbilitySystemLibrary::GetRadialDamageOrigin(EffectContextHandle),
		// 		UBaseAbilitySystemLibrary::GetRadialDamageInnerRadius(EffectContextHandle),
		// 		UBaseAbilitySystemLibrary::GetRadialDamageOuterRadius(EffectContextHandle),
		// 		1.f,
		// 		UDamageType::StaticClass(),
		// 		TArray<AActor*>(),
		// 		SourceAvatar, nullptr);
		// }
		
		Damage += DamageTypeValue;
	}

	// Calculating Base Damage with Strength
	float SourceStrength = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(MonkDamageStatics().StrengthDef, EvaluationParameters, SourceStrength);
	SourceStrength = FMath::Max<float>(0.f, SourceStrength);
	
	float SourcePhysicalAttack = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(MonkDamageStatics().PhysicalAttackDef, EvaluationParameters, SourcePhysicalAttack);
	SourcePhysicalAttack = FMath::Max<float>(0.f, SourcePhysicalAttack);

	Damage += SourcePhysicalAttack + ((SourcePhysicalAttack + SourceCharacterLevel) / 32.f) * ((SourcePhysicalAttack + SourceCharacterLevel) / 32.f);

	// Capture Block Chance on Target for a successful Block
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(MonkDamageStatics().BlockChanceDef, EvaluationParameters, TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(0.f, TargetBlockChance);

	//  Half damage if blocked
	const bool bBlocked = FMath::RandRange(1, 100) < TargetBlockChance;

	UBaseAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle, bBlocked);
	
	Damage = bBlocked ? Damage /  2.f : Damage;

	// 
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(MonkDamageStatics().ArmorDef, EvaluationParameters, TargetArmor);
	TargetArmor = FMath::Max<float>(0.f, TargetArmor);
	
	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(MonkDamageStatics().ArmorPenetrationDef, EvaluationParameters, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max<float>(0.f, SourceArmorPenetration);

	// Armor Penetration
	const UCharacterClassInfo* CharacterClassInfo = UBaseAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
	const FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"), FString());
	const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourceCharacterLevel);
	
	// Effective Armor
	const float EffectiveArmor = TargetArmor * ( 100 - SourceArmorPenetration * ArmorPenetrationCoefficient) / 100.f;
	const FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmor"), FString());
	const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetCharacterLevel);
	Damage *= ( 100 - EffectiveArmor * EffectiveArmorCoefficient ) / 100.f;

	// Critical Hit Chance
	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(MonkDamageStatics().CriticalHitChanceDef, EvaluationParameters, SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(0.f, SourceCriticalHitChance);
	
	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(MonkDamageStatics().ArmorPenetrationDef, EvaluationParameters, TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max<float>(0.f, TargetCriticalHitResistance);
	
	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(MonkDamageStatics().ArmorPenetrationDef, EvaluationParameters, SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max<float>(0.f, SourceCriticalHitDamage);

	const FRealCurve* CriticalHitResistanceCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("CriticalHitResistance"), FString());
	const float CriticalHitResistanceCoefficient = CriticalHitResistanceCurve->Eval(TargetCharacterLevel);

	// Critical Hit Resistance flat reduces Critical Chance percentage
	const float EffectiveCriticalHitChance = SourceCriticalHitChance  - TargetCriticalHitResistance * CriticalHitResistanceCoefficient;

	// Critical Hit Damage increases Critical Damage as additional bonus 
	const bool bCriticalHit = FMath::RandRange(1, 100) < EffectiveCriticalHitChance;
	UBaseAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCriticalHit);
	
	Damage = bCriticalHit ? 2.f * Damage + SourceCriticalHitDamage : Damage; 
	
	const FGameplayModifierEvaluatedData EvaluatedData(UBaseAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}

void UExecCalc_MonkDamage::DetermineStatusEffect(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectSpec& Spec, FAggregatorEvaluateParameters EvaluationParameters,
		const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& TagsToDefs) const
{
	const FBaseGameplayTags GameplayTags = FBaseGameplayTags::Get();
	for (TTuple<FGameplayTag, FGameplayTag> Pair : GameplayTags.DamageTypesToStatusEffects)
	{
		const FGameplayTag& DamageType = Pair.Key;
		const FGameplayTag& StatusEffectType = Pair.Value;
		const float TypeDamage = Spec.GetSetByCallerMagnitude(DamageType, false, -1.f);
		if(TypeDamage > -.5f) // .5f padding for floating point imprecision
		{
			const float SourceStatusEffectChance = Spec.GetSetByCallerMagnitude(GameplayTags.StatusEffect_Chance, false, -1.f);
			float TargetStatusEffectResistance = 0.f;
			const FGameplayTag& ResistanceTag = GameplayTags.DamageTypesToResistances[DamageType];
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(TagsToDefs[ResistanceTag], EvaluationParameters, TargetStatusEffectResistance);
			TargetStatusEffectResistance = FMath::Max<float>(0.f, TargetStatusEffectResistance);
			const float EffectiveStatusEffectChance = SourceStatusEffectChance * ( 100 - TargetStatusEffectResistance ) / 100.f;
			const bool bStatusEffect = FMath::RandRange(1, 100) < EffectiveStatusEffectChance;
			if(bStatusEffect)
			{
				FGameplayEffectContextHandle ContextHandle = Spec.GetContext();
				UBaseAbilitySystemLibrary::SetIsSuccessfulStatusEffect(ContextHandle, true);
				UBaseAbilitySystemLibrary::SetDamageType(ContextHandle, DamageType);

				const float StatusEffectDamage = Spec.GetSetByCallerMagnitude(GameplayTags.StatusEffect_Damage, false, -1.f);
				const float StatusEffectDuration = Spec.GetSetByCallerMagnitude(GameplayTags.StatusEffect_Duration, false, -1.f);
				const float StatusEffectFrequency = Spec.GetSetByCallerMagnitude(GameplayTags.StatusEffect_Frequency, false, -1.f);

				UBaseAbilitySystemLibrary::SetStatusEffectDamage(ContextHandle, StatusEffectDamage);
				UBaseAbilitySystemLibrary::SetStatusEffectDuration(ContextHandle, StatusEffectDuration);
				UBaseAbilitySystemLibrary::SetStatusEffectFrequency(ContextHandle, StatusEffectFrequency);
			}
		}
	}
}