// Retropsis @ 2024

#include "AbilitySystem/BaseAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "BaseGameplayTags.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/BaseAbilitySystemLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/PlayerInterface.h"
#include "Net/UnrealNetwork.h"
#include "Player/PlayerCharacterController.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"

UBaseAttributeSet::UBaseAttributeSet() {}

void UBaseAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Vitality, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Spirit, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Dexterity, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Luck, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, PhysicalAttack, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, MagicalAttack, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, BonusPhysicalAttack, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, BonusMagicalAttack, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, BonusArmor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, BonusArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, BonusBlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, BonusCriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, BonusCriticalHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, BonusCriticalHitResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, BonusHealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, BonusManaRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, BonusMaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, BonusMaxMana, COND_None, REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, PhysicalResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, BluntResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, CuttingResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, PierceResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, FireResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, IceResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, WindResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, LightningResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, HolyResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, DarkResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, NoxiousResistance, COND_None, REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Mana, COND_None, REPNOTIFY_Always);
}

void UBaseAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	if(Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	if(Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
}

void UBaseAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	FEffectProperties Props;
	SetEffectProperties(Data, Props);

	if (Props.TargetCharacter->Implements<UCombatInterface>() && ICombatInterface::Execute_IsDead(Props.TargetCharacter)) return;

	if(Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
			SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	if(Data.EvaluatedData.Attribute == GetManaAttribute())
	{
			SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}
	if(Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		HandleIncomingDamage(Props);
	}
	if(Data.EvaluatedData.Attribute == GetIncomingHealingAttribute())
	{
		HandleIncomingHealing(Props);
	}
	if(Data.EvaluatedData.Attribute == GetIncomingXPAttribute())
	{
		HandleIncomingXP(Props);
	}
}

void UBaseAttributeSet::HandleIncomingDamage(const FEffectProperties& Props)
{
	const float LocalIncomingDamage = GetIncomingDamage();
	SetIncomingDamage(0.f);
	if(LocalIncomingDamage > 0.f)
	{
		const float NewHealth = GetHealth() - LocalIncomingDamage;
		SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

		const bool bFatal = NewHealth <= 0.f;
		if(bFatal)
		{
			if(ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.TargetAvatarActor))
			{
				CombatInterface->Die(UBaseAbilitySystemLibrary::GetDeathImpulse(Props.EffectContextHandle));	
			}
			SendXPEvent(Props);
		}
		else
		{
			if(Props.TargetCharacter->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsElectrocuted(Props.TargetCharacter))
			{
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(FBaseGameplayTags::Get().Effects_HitReact);
				Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);
			}

			const FVector& AirborneForce = UBaseAbilitySystemLibrary::GetAirborneForce(Props.EffectContextHandle);
			if(!AirborneForce.IsNearlyZero())
			{
				Props.TargetCharacter->GetCharacterMovement()->StopMovementImmediately();
				Props.TargetCharacter->LaunchCharacter(AirborneForce, false, true);
			}
		}
		const bool bBlockedHit = UBaseAbilitySystemLibrary::IsBlockedHit(Props.EffectContextHandle);
		const bool bCriticalHit = UBaseAbilitySystemLibrary::IsCriticalHit(Props.EffectContextHandle);
		ShowDamageText(Props, LocalIncomingDamage, bBlockedHit, bCriticalHit);

		if(UBaseAbilitySystemLibrary::IsSuccessfulStatusEffect(Props.EffectContextHandle))
		{
			HandleIncomingStatusEffect(Props);
		}
	}
}

void UBaseAttributeSet::HandleIncomingHealing(const FEffectProperties& Props)
{
	const float LocalIncomingHealing = GetIncomingHealing();
	SetIncomingHealing(0.f);
	if(LocalIncomingHealing > 0.f)
	{
		const float NewHealth = GetHealth() + LocalIncomingHealing;
		SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

		// const bool bFatal = NewHealth <= 0.f;
		// if(bFatal)
		// {
		// 	if(ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.TargetAvatarActor))
		// 	{
		// 		CombatInterface->Die(UBaseAbilitySystemLibrary::GetDeathImpulse(Props.EffectContextHandle));	
		// 	}
		// 	SendXPEvent(Props);
		// }
		// else
		// {
		// 	if(Props.TargetCharacter->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsElectrocuted(Props.TargetCharacter))
		// 	{
		// 		FGameplayTagContainer TagContainer;
		// 		TagContainer.AddTag(FBaseGameplayTags::Get().Effects_HitReact);
		// 		Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);
		// 	}
		//
		// 	const FVector& AirborneForce = UBaseAbilitySystemLibrary::GetAirborneForce(Props.EffectContextHandle);
		// 	if(!AirborneForce.IsNearlyZero())
		// 	{
		// 		Props.TargetCharacter->GetCharacterMovement()->StopMovementImmediately();
		// 		Props.TargetCharacter->LaunchCharacter(AirborneForce, false, true);
		// 	}
		// }
		// const bool bBlockedHit = UBaseAbilitySystemLibrary::IsBlockedHit(Props.EffectContextHandle);
		// const bool bCriticalHit = UBaseAbilitySystemLibrary::IsCriticalHit(Props.EffectContextHandle);
		
		ShowHealingText(Props, LocalIncomingHealing, false);

		// if(UBaseAbilitySystemLibrary::IsSuccessfulStatusEffect(Props.EffectContextHandle))
		// {
		// 	HandleIncomingStatusEffect(Props);
		// }
	}
}

void UBaseAttributeSet::HandleIncomingXP(const FEffectProperties& Props)
{
	const float LocalIncomingXP = GetIncomingXP();
	SetIncomingXP(0.f);
	
	if (Props.SourceCharacter->Implements<UPlayerInterface>() && Props.SourceCharacter->Implements<UCombatInterface>())
	{
		const int32 CurrentLevel = ICombatInterface::Execute_GetCharacterLevel(Props.SourceCharacter);
		const int32 CurrentXP = IPlayerInterface::Execute_GetXP(Props.SourceCharacter);

		const int32 NewLevel = IPlayerInterface::Execute_FindLevelForXP(Props.SourceCharacter, CurrentXP + LocalIncomingXP);
		const int32 NumLevelUps = NewLevel - CurrentLevel;
		if(NumLevelUps > 0)
		{
			IPlayerInterface::Execute_AddToPlayerLevel(Props.SourceCharacter, NumLevelUps);
			
			int32 AttributePointReward = 0;
			int32 SpellPointReward = 0;

			for (int i = 0; i < NumLevelUps; ++i)
			{
				SpellPointReward += IPlayerInterface::Execute_GetSpellPointReward(Props.SourceCharacter, CurrentLevel + i);
				AttributePointReward += IPlayerInterface::Execute_GetAttributePointReward(Props.SourceCharacter, CurrentLevel + i);
			}
			
			IPlayerInterface::Execute_AddToAttributePoints(Props.SourceCharacter, AttributePointReward);
			IPlayerInterface::Execute_AddToSpellPoints(Props.SourceCharacter, SpellPointReward);

			bTopOffHealth = true;
			bTopOffMana = true;
				
			IPlayerInterface::Execute_LevelUp(Props.SourceCharacter);
		}
		IPlayerInterface::Execute_AddToXP(Props.SourceCharacter, LocalIncomingXP);
	}
}

void UBaseAttributeSet::HandleIncomingStatusEffect(const FEffectProperties& Props)
{
	FGameplayEffectContextHandle EffectContext = Props.SourceASC->MakeEffectContext();
	EffectContext.AddSourceObject(Props.SourceAvatarActor);

	FBaseGameplayTags GameplayTags = FBaseGameplayTags::Get();
	const FGameplayTag DamageType = UBaseAbilitySystemLibrary::GetStatusEffectDamageType(Props.EffectContextHandle);
	const float StatusEffectDamage = UBaseAbilitySystemLibrary::GetStatusEffectDamage(Props.EffectContextHandle);
	const float StatusEffectDuration = UBaseAbilitySystemLibrary::GetStatusEffectDuration(Props.EffectContextHandle);
	const float StatusEffectFrequency = UBaseAbilitySystemLibrary::GetStatusEffectFrequency(Props.EffectContextHandle);
	
	const FString StatusEffectName = FString::Printf(TEXT("DynamycStatusEffect_%s"), *DamageType.ToString());
	UGameplayEffect* Effect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(StatusEffectName));
	Effect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
	Effect->Period = StatusEffectFrequency;
	Effect->DurationMagnitude = FScalableFloat(StatusEffectDuration);
	Effect->StackingType = EGameplayEffectStackingType::AggregateBySource;
	Effect->StackLimitCount = 1;
	// Effect->InheritableOwnedTagsContainer.AddTag(DamageType);

	// 5.3+ adding to CombinedTags
	FInheritedTagContainer TagContainer = FInheritedTagContainer();
	UTargetTagsGameplayEffectComponent& Component = Effect->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	TagContainer.Added.AddTag(GameplayTags.DamageTypesToStatusEffects[DamageType]);
	// TagContainer.CombinedTags.AddTag(GameplayTags.DamageTypesToStatusEffects[DamageType]);

	const FGameplayTag StatusEffectTag = GameplayTags.DamageTypesToStatusEffects[DamageType];
	if(StatusEffectTag.MatchesTagExact(GameplayTags.StatusEffect_Stun))
	{
		TagContainer.Added.AddTag(GameplayTags.Player_Block_CursorTrace);
		// TagContainer.CombinedTags.AddTag(GameplayTags.Player_Block_CursorTrace);
		
		TagContainer.Added.AddTag(GameplayTags.Player_Block_InputHeld);
		// TagContainer.CombinedTags.AddTag(GameplayTags.Player_Block_InputHeld);

		TagContainer.Added.AddTag(GameplayTags.Player_Block_InputPressed);
		// TagContainer.CombinedTags.AddTag(GameplayTags.Player_Block_InputPressed);

		TagContainer.Added.AddTag(GameplayTags.Player_Block_InputReleased);
		// TagContainer.CombinedTags.AddTag(GameplayTags.Player_Block_InputReleased);
	}
	// Applying the TagContainer to Component
	Component.SetAndApplyTargetTagChanges(TagContainer);
	// 5.3 End

	int32 Index = Effect->Modifiers.Num();
	Effect->Modifiers.Add(FGameplayModifierInfo());
	FGameplayModifierInfo& ModifierInfo = Effect->Modifiers[Index];
	ModifierInfo.ModifierMagnitude = FScalableFloat(StatusEffectDamage);
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	ModifierInfo.Attribute = UBaseAttributeSet::GetIncomingDamageAttribute();

	if(FGameplayEffectSpec* MutableSpec = new FGameplayEffectSpec(Effect, EffectContext, 1.f))
	{
		FBaseGameplayEffectContext* BaseContext = static_cast<FBaseGameplayEffectContext*>(MutableSpec->GetContext().Get());
		const TSharedPtr<FGameplayTag> StatusEffectDamageType = MakeShareable(new FGameplayTag(DamageType));
		BaseContext->SetDamageType(StatusEffectDamageType);

		Props.TargetASC->ApplyGameplayEffectSpecToSelf(*MutableSpec);
	}
}

void UBaseAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if(Attribute == GetMaxHealthAttribute() && bTopOffHealth)
	{
		SetHealth(GetMaxHealth());
		bTopOffHealth = false;
	}
	if(Attribute == GetMaxManaAttribute() && bTopOffMana)
	{
		SetMana(GetMaxMana());
		bTopOffMana = false;
	}
}

void UBaseAttributeSet::ShowDamageText(const FEffectProperties& Props, const float Damage, bool bBlockedHit, bool bCriticalHit) const
{
	if(Props.SourceCharacter != Props.TargetCharacter)
	{
		if (APlayerCharacterController* PC = Cast<APlayerCharacterController>(Props.SourceCharacter->Controller))
		{
			const float Delay = UBaseAbilitySystemLibrary::GetShowDamageDelay(Props.EffectContextHandle);
			PC->ClientShowDamageNumber(Damage, Props.TargetCharacter, bBlockedHit, bCriticalHit, Delay);
			return;
		}
		if (APlayerCharacterController* PC = Cast<APlayerCharacterController>(Props.TargetCharacter->Controller))
		{
			const float Delay = UBaseAbilitySystemLibrary::GetShowDamageDelay(Props.EffectContextHandle);
			PC->ClientShowDamageNumber(Damage, Props.TargetCharacter, bBlockedHit, bCriticalHit, Delay);
		}
	}
}

void UBaseAttributeSet::ShowHealingText(const FEffectProperties& Props, float Healing, bool bCriticalHit) const
{
	if(Props.SourceCharacter != Props.TargetCharacter)
	{
		if (APlayerCharacterController* PC = Cast<APlayerCharacterController>(Props.SourceCharacter->Controller))
		{
			// const float Delay = UBaseAbilitySystemLibrary::GetShowDamageDelay(Props.EffectContextHandle);
			PC->ClientShowHealingNumber(Healing, Props.TargetCharacter,  bCriticalHit/*, Delay*/);
			return;
		}
		if (APlayerCharacterController* PC = Cast<APlayerCharacterController>(Props.TargetCharacter->Controller))
		{
			// const float Delay = UBaseAbilitySystemLibrary::GetShowDamageDelay(Props.EffectContextHandle);
			PC->ClientShowHealingNumber(Healing, Props.TargetCharacter, bCriticalHit/*, Delay*/);
		}
	}
}

void UBaseAttributeSet::SendXPEvent(const FEffectProperties& Props)
{
	if (Props.TargetCharacter->Implements<UCombatInterface>())
	{
		const int32 TargetLevel = ICombatInterface::Execute_GetCharacterLevel(Props.TargetCharacter);;
		const ECharacterClass TargetClass = ICombatInterface::Execute_GetCharacterClass(Props.TargetCharacter);
		const int32 XPReward = UBaseAbilitySystemLibrary::GetXPRewardForClassAndLevel(Props.TargetCharacter, TargetClass, TargetLevel);

		const FBaseGameplayTags& GameplayTags = FBaseGameplayTags::Get();
		FGameplayEventData Payload;
		Payload.EventTag = GameplayTags.Attributes_Meta_IncomingXP;
		Payload.EventMagnitude = XPReward;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Props.SourceCharacter, GameplayTags.Attributes_Meta_IncomingXP, Payload);
	}
}

void UBaseAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props)
{
	Props.EffectContextHandle = Data.EffectSpec.GetContext();

	// Source, causer
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();
	
	if(IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if(Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
		{
			if(const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		if(Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}

	// Target, Owner of this AS
	if(Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}

void UBaseAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Strength, OldStrength);
}

void UBaseAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Intelligence, OldIntelligence);
}

void UBaseAttributeSet::OnRep_Vitality(const FGameplayAttributeData& OldVitality) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Vitality, OldVitality);
}

void UBaseAttributeSet::OnRep_Spirit(const FGameplayAttributeData& OldSpirit) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Spirit, OldSpirit);
}

void UBaseAttributeSet::OnRep_Dexterity(const FGameplayAttributeData& OldDexterity) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Dexterity, OldDexterity);
}

void UBaseAttributeSet::OnRep_Luck(const FGameplayAttributeData& OldLuck) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Luck, OldLuck);
}

void UBaseAttributeSet::OnRep_PhysicalAttack(const FGameplayAttributeData& OldPhysicalAttack) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, PhysicalAttack, OldPhysicalAttack);
}

void UBaseAttributeSet::OnRep_MagicalAttack(const FGameplayAttributeData& OldMagicalAttack) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MagicalAttack, OldMagicalAttack);
}

void UBaseAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Armor, OldArmor);
}

void UBaseAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, ArmorPenetration, OldArmorPenetration);
}

void UBaseAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, BlockChance, OldBlockChance);
}

void UBaseAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, CriticalHitChance, OldCriticalHitChance);
}

void UBaseAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, CriticalHitDamage, OldCriticalHitDamage);
}

void UBaseAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, CriticalHitResistance, OldCriticalHitResistance);
}

void UBaseAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxHealth, OldHealthRegeneration);
}

void UBaseAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, ManaRegeneration, OldManaRegeneration);
}

void UBaseAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxHealth, OldMaxHealth);
}

void UBaseAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxMana, OldMaxMana);
}

/*
 * Bonus To Secondary Attributes Rep Notifies
 */
void UBaseAttributeSet::OnRep_BonusPhysicalAttack(const FGameplayAttributeData& OldBonusPhysicalAttack) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, BonusPhysicalAttack, OldBonusPhysicalAttack);
}

void UBaseAttributeSet::OnRep_BonusMagicalAttack(const FGameplayAttributeData& OldBonusMagicalAttack) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, BonusMagicalAttack, OldBonusMagicalAttack);
}

void UBaseAttributeSet::OnRep_BonusArmor(const FGameplayAttributeData& OldBonusArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, BonusArmor, OldBonusArmor);
}

void UBaseAttributeSet::OnRep_BonusArmorPenetration(const FGameplayAttributeData& OldBonusArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, BonusArmorPenetration, OldBonusArmorPenetration);
}

void UBaseAttributeSet::OnRep_BonusBlockChance(const FGameplayAttributeData& OldBonusBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, BonusBlockChance, OldBonusBlockChance);
}

void UBaseAttributeSet::OnRep_BonusCriticalHitChance(const FGameplayAttributeData& OldBonusCriticalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, BonusCriticalHitChance, OldBonusCriticalHitChance);
}

void UBaseAttributeSet::OnRep_BonusCriticalHitDamage(const FGameplayAttributeData& OldBonusCriticalHitDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, BonusCriticalHitDamage, OldBonusCriticalHitDamage);
}

void UBaseAttributeSet::OnRep_BonusCriticalHitResistance(const FGameplayAttributeData& OldBonusCriticalHitResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, BonusCriticalHitResistance, OldBonusCriticalHitResistance);
}

void UBaseAttributeSet::OnRep_BonusHealthRegeneration(const FGameplayAttributeData& OldBonusHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, BonusHealthRegeneration, OldBonusHealthRegeneration);
}

void UBaseAttributeSet::OnRep_BonusManaRegeneration(const FGameplayAttributeData& OldBonusManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, BonusManaRegeneration, OldBonusManaRegeneration);
}

void UBaseAttributeSet::OnRep_BonusMaxHealth(const FGameplayAttributeData& OldBonusMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, BonusMaxHealth, OldBonusMaxHealth);
}

void UBaseAttributeSet::OnRep_BonusMaxMana(const FGameplayAttributeData& OldBonusMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, BonusMaxMana, OldBonusMaxMana);
}

/*
 * Resistances Rep Notifies
 */
void UBaseAttributeSet::OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, PhysicalResistance, OldPhysicalResistance);
}

void UBaseAttributeSet::OnRep_BluntResistance(const FGameplayAttributeData& OldBluntResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, BluntResistance, OldBluntResistance);
}

void UBaseAttributeSet::OnRep_CuttingResistance(const FGameplayAttributeData& OldCuttingResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, CuttingResistance, OldCuttingResistance);
}

void UBaseAttributeSet::OnRep_PierceResistance(const FGameplayAttributeData& OldPierceResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, PierceResistance, OldPierceResistance);
}

void UBaseAttributeSet::OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, FireResistance, OldFireResistance);
}

void UBaseAttributeSet::OnRep_IceResistance(const FGameplayAttributeData& OldIceResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, IceResistance, OldIceResistance);
}

void UBaseAttributeSet::OnRep_WindResistance(const FGameplayAttributeData& OldWindResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, WindResistance, OldWindResistance);
}

void UBaseAttributeSet::OnRep_LightningResistance(const FGameplayAttributeData& OldLightningResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, LightningResistance, OldLightningResistance);
}

void UBaseAttributeSet::OnRep_HolyResistance(const FGameplayAttributeData& OldHolyResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, HolyResistance, OldHolyResistance);
}

void UBaseAttributeSet::OnRep_DarkResistance(const FGameplayAttributeData& OldDarkResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, DarkResistance, OldDarkResistance);
}

void UBaseAttributeSet::OnRep_NoxiousResistance(const FGameplayAttributeData& OldNoxiousResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, NoxiousResistance, OldNoxiousResistance);
}

void UBaseAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Health, OldHealth);
}

void UBaseAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Mana, OldMana);
}
