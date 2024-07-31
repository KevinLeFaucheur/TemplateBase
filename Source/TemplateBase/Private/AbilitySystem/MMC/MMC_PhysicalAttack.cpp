// Retropsis @ 2024


#include "AbilitySystem/MMC/MMC_PhysicalAttack.h"

#include "AbilitySystem/BaseAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_PhysicalAttack::UMMC_PhysicalAttack()
{
	StrengthDef.AttributeToCapture = UBaseAttributeSet::GetStrengthAttribute();
	StrengthDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	StrengthDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(StrengthDef);
	
	DexterityDef.AttributeToCapture = UBaseAttributeSet::GetDexterityAttribute();
	DexterityDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	DexterityDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(DexterityDef);
}

float UMMC_PhysicalAttack::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather Tags from Source and Target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Strength = 0.f;
	GetCapturedAttributeMagnitude(StrengthDef, Spec, EvaluationParameters, Strength);
	Strength = FMath::Max<float>(Strength, 0.f);
	
	float Dexterity = 0.f;
	GetCapturedAttributeMagnitude(DexterityDef, Spec, EvaluationParameters, Dexterity);
	Dexterity = FMath::Max<float>(Dexterity, 0.f);

	float PhysicalAttack = 0.f;
	if (Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
	{
		PhysicalAttack = ICombatInterface::Execute_GetWeaponPhysicalAttack(Spec.GetContext().GetSourceObject());
	}
	
	return Strength + Dexterity + PhysicalAttack;
}
