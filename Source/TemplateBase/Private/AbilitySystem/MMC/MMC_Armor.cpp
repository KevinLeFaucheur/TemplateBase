// Retropsis @ 2024

#include "AbilitySystem/MMC/MMC_Armor.h"

#include "AbilitySystem/BaseAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_Armor::UMMC_Armor()
{
	VitalityDef.AttributeToCapture = UBaseAttributeSet::GetVitalityAttribute();
	VitalityDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VitalityDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(VitalityDef);
	
	BonusArmorDef.AttributeToCapture = UBaseAttributeSet::GetBonusArmorAttribute();
	BonusArmorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	BonusArmorDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(BonusArmorDef);
}

float UMMC_Armor::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather Tags from Source and Target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Vitality = 0.f;
	GetCapturedAttributeMagnitude(VitalityDef, Spec, EvaluationParameters, Vitality);
	Vitality = FMath::Max<float>(Vitality, 0.f);
	
	float BonusArmor = 0.f;
	GetCapturedAttributeMagnitude(BonusArmorDef, Spec, EvaluationParameters, BonusArmor);
	// BonusArmor = FMath::Max<float>(BonusArmor, 0.f);

	// float MagicalAttack = 0.f;
	// if (Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
	// {
	// 	MagicalAttack = ICombatInterface::Execute_GetArmorValues(Spec.GetContext().GetSourceObject());
	// }
	
	return Vitality + BonusArmor;
}
