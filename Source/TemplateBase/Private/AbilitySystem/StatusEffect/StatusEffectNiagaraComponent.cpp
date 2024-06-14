// Retropsis @ 2024


#include "AbilitySystem/StatusEffect/StatusEffectNiagaraComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"

UStatusEffectNiagaraComponent::UStatusEffectNiagaraComponent()
{
	bAutoActivate = false;
}

void UStatusEffectNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();
	
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner());
	if(UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner()))
	{
		ASC->RegisterGameplayTagEvent(StatusEffectTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UStatusEffectNiagaraComponent::StatusEffectTagChanged);
	}
	else if (CombatInterface)
	{
		CombatInterface->GetOnASCRegistered().AddWeakLambda(this, [this] (UAbilitySystemComponent* InASC) 
		{
			InASC->RegisterGameplayTagEvent(StatusEffectTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UStatusEffectNiagaraComponent::StatusEffectTagChanged);
		});
	}
	if(CombatInterface)
	{
		CombatInterface->GetOnDeathDelegate().AddDynamic(this, &UStatusEffectNiagaraComponent::OnOwnerDead);
	}
}

void UStatusEffectNiagaraComponent::StatusEffectTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	const bool bOwnerValid = IsValid(GetOwner());
	const bool bOwnerAlive = GetOwner()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(GetOwner());
	if(NewCount > 0 && bOwnerValid && bOwnerAlive)
	{
		Activate();
	}
	else
	{
		Deactivate();
	}
}

void UStatusEffectNiagaraComponent::OnOwnerDead(AActor* DeadActor)
{
	Deactivate();
}
