// Retropsis @ 2024

#include "AbilitySystem/MMC/MMC_MaxHealth.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	VitalityDef.AttributeToCapture = UBaseAttributeSet::GetVitalityAttribute();
	VitalityDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VitalityDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(VitalityDef);
	
	BonusMaxHealthDef.AttributeToCapture = UBaseAttributeSet::GetBonusMaxHealthAttribute();
	BonusMaxHealthDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	BonusMaxHealthDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(BonusMaxHealthDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
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
	
	float BonusMaxHealth = 0.f;
	GetCapturedAttributeMagnitude(BonusMaxHealthDef, Spec, EvaluationParameters, BonusMaxHealth);
	BonusMaxHealth = FMath::Max<float>(BonusMaxHealth, 0.f);

	int32 PlayerLevel = 1;
	if (Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
	{
		PlayerLevel = ICombatInterface::Execute_GetCharacterLevel(Spec.GetContext().GetSourceObject());
	}
	
	return 80.f + 2.5f * Vitality + 10.f * PlayerLevel + BonusMaxHealth;
}
