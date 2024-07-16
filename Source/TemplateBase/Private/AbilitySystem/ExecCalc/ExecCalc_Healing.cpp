// Retropsis @ 2024

#include "AbilitySystem/ExecCalc/ExecCalc_Healing.h"

#include "BaseGameplayTags.h"
#include "AbilitySystem/BaseAbilitySystemLibrary.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "Interaction/CombatInterface.h"

struct BaseHealingStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MaxHealth);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Intelligence);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Spirit);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Vitality);
	
	BaseHealingStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, Health, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, MaxHealth, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, Vitality, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, Intelligence, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, Spirit, Source, false);
	}
};

static const BaseHealingStatics& HealingStatics()
{
	static  BaseHealingStatics HStatics;
	return HStatics;
}

UExecCalc_Healing::UExecCalc_Healing()
{
	RelevantAttributesToCapture.Add(HealingStatics().HealthDef);
	RelevantAttributesToCapture.Add(HealingStatics().MaxHealthDef);
	RelevantAttributesToCapture.Add(HealingStatics().VitalityDef);
	RelevantAttributesToCapture.Add(HealingStatics().IntelligenceDef);
	RelevantAttributesToCapture.Add(HealingStatics().SpiritDef);
}

void UExecCalc_Healing::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);
	
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

	// TODO: Eventually adding some Healing/Defensive Status Effects like Regen, Barrier/Shield	
	// Status Effects
	// DetermineStatusEffect(ExecutionParams, Spec, EvaluationParameters, TagsToCaptureDefs);
	
	/*
	 * Get Healing Set By Caller Magnitude
	 */
	float Healing = 0.f;
	
	float HealingTypeValue = Spec.GetSetByCallerMagnitude(FBaseGameplayTags::Get().Healing_Health, false);
	
	float Resistance = 0.f;
	// ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvaluationParameters, Resistance);
	// Resistance = FMath::Clamp(Resistance, 0.f, 100.f);

	HealingTypeValue *= ( 100.f - Resistance ) / 100.f;

	// TODO: Would need Radial Healing as well
	if (UBaseAbilitySystemLibrary::IsRadialHealing(EffectContextHandle))
	{
		HealingTypeValue = UBaseAbilitySystemLibrary::GetRadialHealingWithFalloff(
		   TargetAvatar,
		   HealingTypeValue,
		   0.f,
		   UBaseAbilitySystemLibrary::GetRadialHealingOrigin(EffectContextHandle),
		   UBaseAbilitySystemLibrary::GetRadialHealingInnerRadius(EffectContextHandle),
		   UBaseAbilitySystemLibrary::GetRadialHealingOuterRadius(EffectContextHandle),
		   1.f);
	}
	
	Healing += HealingTypeValue;
	
	// Capture Intelligence on Source for healing power
	float SourceIntelligence = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(HealingStatics().IntelligenceDef, EvaluationParameters, SourceIntelligence);
	SourceIntelligence = FMath::Max<float>(0.f, SourceIntelligence);
	
	// Capture Spirit on Source for healing power
	float SourceSpirit = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(HealingStatics().SpiritDef, EvaluationParameters, SourceSpirit);
	SourceSpirit = FMath::Max<float>(0.f, SourceSpirit);

	Healing = HealingTypeValue/* * 22.0*/ + ((SourceCharacterLevel + SourceIntelligence * 0.5f + SourceSpirit) * 6.0);
	
	// Capture Vitality on Target for healing power
	float TargetVitality = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(HealingStatics().IntelligenceDef, EvaluationParameters, TargetVitality);
	TargetVitality = FMath::Max<float>(0.f, TargetVitality);
	
	// Capture Health on Target for healing power
	float TargetHealth = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(HealingStatics().HealthDef, EvaluationParameters, TargetHealth);
	TargetHealth = FMath::Max<float>(0.f, TargetHealth);
	
	// Capture MaxHealth on Target for healing power
	float TargetMaxHealth = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(HealingStatics().MaxHealthDef, EvaluationParameters, TargetMaxHealth);
	TargetMaxHealth = FMath::Max<float>(0.f, TargetMaxHealth);
	
	const FGameplayModifierEvaluatedData EvaluatedData(UBaseAttributeSet::GetIncomingHealingAttribute(), EGameplayModOp::Additive, Healing);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
