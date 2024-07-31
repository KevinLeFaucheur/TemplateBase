// Retropsis @ 2024

#include "AbilitySystem/MMC/MMC_MagicalAttack.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MagicalAttack::UMMC_MagicalAttack()
{
	IntelligenceDef.AttributeToCapture = UBaseAttributeSet::GetIntelligenceAttribute();
	IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntelligenceDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(IntelligenceDef);
	
	SpiritDef.AttributeToCapture = UBaseAttributeSet::GetSpiritAttribute();
	SpiritDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	SpiritDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(SpiritDef);
}

float UMMC_MagicalAttack::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather Tags from Source and Target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Intelligence = 0.f;
	GetCapturedAttributeMagnitude(IntelligenceDef, Spec, EvaluationParameters, Intelligence);
	Intelligence = FMath::Max<float>(Intelligence, 0.f);
	
	float Spirit = 0.f;
	GetCapturedAttributeMagnitude(SpiritDef, Spec, EvaluationParameters, Spirit);
	Spirit = FMath::Max<float>(Spirit, 0.f);

	float MagicalAttack = 0.f;
	if (Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
	{
		MagicalAttack = ICombatInterface::Execute_GetWeaponMagicalAttack(Spec.GetContext().GetSourceObject());
	}
	
	return Intelligence + Spirit + MagicalAttack;
}
