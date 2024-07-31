// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "BaseAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties() {}
	
	FGameplayEffectContextHandle EffectContextHandle;

	UPROPERTY() UAbilitySystemComponent* SourceASC = nullptr;
	UPROPERTY() AActor* SourceAvatarActor = nullptr;
	UPROPERTY() AController* SourceController = nullptr;
	UPROPERTY() ACharacter* SourceCharacter = nullptr;
	
	UPROPERTY() UAbilitySystemComponent* TargetASC = nullptr;
	UPROPERTY() AActor* TargetAvatarActor = nullptr;
	UPROPERTY() AController* TargetController = nullptr;
	UPROPERTY() ACharacter* TargetCharacter = nullptr;
};


// typedef TBaseStaticDelegateInstance<FGameplayAttribute(), FDefaultDelegateUserPolicy>::FFuncPtr FAttributeFuncPtr;
template<class T>
using TStaticFuncPtr = typename  TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;

/**
 * 
 */
UCLASS()
class TEMPLATEBASE_API UBaseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UBaseAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	/*
	 * Signature for a static function pointer such as "UBaseAttributeSet::GetStrengthAttribute" 
	 * TBaseStaticDelegateInstance<FGameplayAttribute(), FDefaultDelegateUserPolicy>::FFuncPtr
	 *  Reduced to FGameplayAttribute(*)()
	 */
	// TMap<FGameplayTag, TBaseStaticDelegateInstance<FGameplayAttribute(), FDefaultDelegateUserPolicy>::FFuncPtr> TagsToAttributes;
	// TMap<FGameplayTag, FGameplayAttribute(*)()> TagsToAttributes;
	// TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>> TagsToAttributes;
	
	/*
	 * Primary Attributes
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Strength, Category="Primary Attributes")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Strength);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Intelligence, Category="Primary Attributes")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Intelligence);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Vitality, Category="Primary Attributes")
	FGameplayAttributeData Vitality;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Vitality);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Spirit, Category="Primary Attributes")
	FGameplayAttributeData Spirit;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Spirit);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Dexterity, Category="Primary Attributes")
	FGameplayAttributeData Dexterity;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Dexterity);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Luck, Category="Primary Attributes")
	FGameplayAttributeData Luck;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Luck);
	
	/*
	 * Secondary Attributes
	*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_PhysicalAttack, Category="Secondary Attributes")
	FGameplayAttributeData PhysicalAttack;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, PhysicalAttack);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MagicalAttack, Category="Secondary Attributes")
	FGameplayAttributeData MagicalAttack;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MagicalAttack);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Armor, Category="Secondary Attributes")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Armor);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_ArmorPenetration, Category="Secondary Attributes")
	FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, ArmorPenetration);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_BlockChance, Category="Secondary Attributes")
	FGameplayAttributeData BlockChance;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, BlockChance);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CriticalHitChance, Category="Secondary Attributes")
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, CriticalHitChance);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CriticalHitDamage, Category="Secondary Attributes")
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, CriticalHitDamage);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CriticalHitResistance, Category="Secondary Attributes")
	FGameplayAttributeData CriticalHitResistance;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, CriticalHitResistance);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_HealthRegeneration, Category="Secondary Attributes")
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, HealthRegeneration);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_ManaRegeneration, Category="Secondary Attributes")
	FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, ManaRegeneration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxHealth, Category="Secondary Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxMana, Category="Secondary Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxMana);
		
	/*
	 * Bonus To Secondary Attributes
	*/	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_BonusPhysicalAttack, Category="Secondary Attributes")
	FGameplayAttributeData BonusPhysicalAttack;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, BonusPhysicalAttack);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_BonusMagicalAttack, Category="Secondary Attributes")
	FGameplayAttributeData BonusMagicalAttack;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, BonusMagicalAttack);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_BonusArmor, Category="Secondary Attributes")
	FGameplayAttributeData BonusArmor;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, BonusArmor);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_BonusArmorPenetration, Category="Secondary Attributes")
	FGameplayAttributeData BonusArmorPenetration;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, BonusArmorPenetration);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_BonusBlockChance, Category="Secondary Attributes")
	FGameplayAttributeData BonusBlockChance;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, BonusBlockChance);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_BonusCriticalHitChance, Category="Secondary Attributes")
	FGameplayAttributeData BonusCriticalHitChance;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, BonusCriticalHitChance);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_BonusCriticalHitDamage, Category="Secondary Attributes")
	FGameplayAttributeData BonusCriticalHitDamage;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, BonusCriticalHitDamage);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_BonusCriticalHitResistance, Category="Secondary Attributes")
	FGameplayAttributeData BonusCriticalHitResistance;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, BonusCriticalHitResistance);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_BonusHealthRegeneration, Category="Secondary Attributes")
	FGameplayAttributeData BonusHealthRegeneration;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, BonusHealthRegeneration);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_BonusManaRegeneration, Category="Secondary Attributes")
	FGameplayAttributeData BonusManaRegeneration;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, BonusManaRegeneration);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_BonusMaxHealth, Category="Secondary Attributes")
	FGameplayAttributeData BonusMaxHealth;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, BonusMaxHealth);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_BonusMaxMana, Category="Secondary Attributes")
	FGameplayAttributeData BonusMaxMana;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, BonusMaxMana);
	
	/*
	 * Resistance Attributes
	*/
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_PhysicalResistance, Category="Resistance Attributes")
	FGameplayAttributeData PhysicalResistance;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, PhysicalResistance);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_BluntResistance, Category="Resistance Attributes")
	FGameplayAttributeData BluntResistance;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, BluntResistance);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CuttingResistance, Category="Resistance Attributes")
	FGameplayAttributeData CuttingResistance;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, CuttingResistance);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_PierceResistance, Category="Resistance Attributes")
	FGameplayAttributeData PierceResistance;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, PierceResistance);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_FireResistance, Category="Resistance Attributes")
	FGameplayAttributeData FireResistance;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, FireResistance);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_IceResistance, Category="Resistance Attributes")
	FGameplayAttributeData IceResistance;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, IceResistance);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_WindResistance, Category="Resistance Attributes")
	FGameplayAttributeData WindResistance;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, WindResistance);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_LightningResistance, Category="Resistance Attributes")
	FGameplayAttributeData LightningResistance;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, LightningResistance);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_HolyResistance, Category="Resistance Attributes")
	FGameplayAttributeData HolyResistance;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, HolyResistance);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_DarkResistance, Category="Resistance Attributes")
	FGameplayAttributeData DarkResistance;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, DarkResistance);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_NoxiousResistance, Category="Resistance Attributes")
	FGameplayAttributeData NoxiousResistance;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, NoxiousResistance);
	
	/*
	 * Vital Attributes
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Health, Category="Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Health);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Mana, Category="Vital Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Mana);

	/*
	 * Meta Attributes
	 */
	UPROPERTY(BlueprintReadOnly, Category="Meta Attributes")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, IncomingDamage);
	
	UPROPERTY(BlueprintReadOnly, Category="Meta Attributes")
	FGameplayAttributeData IncomingHealing;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, IncomingHealing);
	
	UPROPERTY(BlueprintReadOnly, Category="Meta Attributes")
	FGameplayAttributeData IncomingXP;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, IncomingXP);

	/*
	 * Primary Attributes Rep Notifies
	 */
	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;

	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;

	UFUNCTION()
	void OnRep_Vitality(const FGameplayAttributeData& OldVitality) const;

	UFUNCTION()
	void OnRep_Spirit(const FGameplayAttributeData& OldSpirit) const;

	UFUNCTION()
	void OnRep_Dexterity(const FGameplayAttributeData& OldDexterity) const;

	UFUNCTION()
	void OnRep_Luck(const FGameplayAttributeData& OldLuck) const;

	/*
	 * Secondary Attributes Rep Notifies
	 */
	UFUNCTION()
	void OnRep_PhysicalAttack(const FGameplayAttributeData& OldPhysicalAttack) const;
	
	UFUNCTION()
	void OnRep_MagicalAttack(const FGameplayAttributeData& OldMagicalAttack) const;
	
	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldArmor) const;
	
	UFUNCTION()
	void OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const;
	
	UFUNCTION()
	void OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const;
	
	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const;
	
	UFUNCTION()
	void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const;
	
	UFUNCTION()
	void OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const;
	
	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const;
	
	UFUNCTION()
	void OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;
	
	/*
	 * Bonus To Secondary Attributes Rep Notifies
	*/
	UFUNCTION()
	void OnRep_BonusPhysicalAttack(const FGameplayAttributeData& OldBonusPhysicalAttack) const;
	
	UFUNCTION()
	void OnRep_BonusMagicalAttack(const FGameplayAttributeData& OldBonusMagicalAttack) const;
	
	UFUNCTION()
	void OnRep_BonusArmor(const FGameplayAttributeData& OldBonusArmor) const;
	
	UFUNCTION()
	void OnRep_BonusArmorPenetration(const FGameplayAttributeData& OldBonusArmorPenetration) const;
	
	UFUNCTION()
	void OnRep_BonusBlockChance(const FGameplayAttributeData& OldBonusBlockChance) const;
	
	UFUNCTION()
	void OnRep_BonusCriticalHitChance(const FGameplayAttributeData& OldBonusCriticalHitChance) const;
	
	UFUNCTION()
	void OnRep_BonusCriticalHitDamage(const FGameplayAttributeData& OldBonusCriticalHitDamage) const;
	
	UFUNCTION()
	void OnRep_BonusCriticalHitResistance(const FGameplayAttributeData& OldBonusCriticalHitResistance) const;
	
	UFUNCTION()
	void OnRep_BonusHealthRegeneration(const FGameplayAttributeData& OldBonusHealthRegeneration) const;
	
	UFUNCTION()
	void OnRep_BonusManaRegeneration(const FGameplayAttributeData& OldBonusManaRegeneration) const;

	UFUNCTION()
	void OnRep_BonusMaxHealth(const FGameplayAttributeData& OldBonusMaxHealth) const;

	UFUNCTION()
	void OnRep_BonusMaxMana(const FGameplayAttributeData& OldBonusMaxMana) const;
	
	/*
	 * Resistance Attributes Rep Notifies
	 */
	UFUNCTION()
	void OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance) const;
	
	UFUNCTION()
	void OnRep_BluntResistance(const FGameplayAttributeData& OldBluntResistance) const;
	
	UFUNCTION()
	void OnRep_CuttingResistance(const FGameplayAttributeData& OldCuttingResistance) const;
	
	UFUNCTION()
	void OnRep_PierceResistance(const FGameplayAttributeData& OldPierceResistance) const;
	
	UFUNCTION()
	void OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance) const;
	
	UFUNCTION()
	void OnRep_IceResistance(const FGameplayAttributeData& OldIceResistance) const;
	
	UFUNCTION()
	void OnRep_WindResistance(const FGameplayAttributeData& OldWindResistance) const;
	
	UFUNCTION()
	void OnRep_LightningResistance(const FGameplayAttributeData& OldLightningResistance) const;
	
	UFUNCTION()
	void OnRep_HolyResistance(const FGameplayAttributeData& OldHolyResistance) const;
	
	UFUNCTION()
	void OnRep_DarkResistance(const FGameplayAttributeData& OldDarkResistance) const;
	
	UFUNCTION()
	void OnRep_NoxiousResistance(const FGameplayAttributeData& OldNoxiousResistance) const;
	
	/*
	 * Vital Attributes Rep Notifies
	 */
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props);
	void HandleIncomingDamage(const FEffectProperties& Props);
	void HandleIncomingHealing(const FEffectProperties& Props);
	void HandleIncomingXP(const FEffectProperties& Props);
	void HandleIncomingStatusEffect(const FEffectProperties& Props);
	void ShowDamageText(const FEffectProperties& Props, float Damage, bool bBlockedHit, bool bCriticalHit) const;
	void ShowHealingText(const FEffectProperties& Props, float Healing, bool bCriticalHit) const;
	void SendXPEvent(const FEffectProperties& Props);

	bool bTopOffHealth = false;
	bool bTopOffMana = false;
};
