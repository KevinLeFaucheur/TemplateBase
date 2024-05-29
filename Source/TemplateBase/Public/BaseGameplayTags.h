// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 *  Single for Native Gameplay tags
 */
struct  FBaseGameplayTags
{
public:
static const FBaseGameplayTags& Get() { return GameplayTags; }
static  void InitializeNativeGameplayTags();

 /*
  * Vital Attributes
 */
	
 /*
  * Primary Attributes
  */
 FGameplayTag Attributes_Primary_Strength;
 FGameplayTag Attributes_Primary_Intelligence;
 FGameplayTag Attributes_Primary_Vitality;
 FGameplayTag Attributes_Primary_Spirit;
 FGameplayTag Attributes_Primary_Dexterity;
 FGameplayTag Attributes_Primary_Luck;
	
 /*
  * Secondary Attributes
  */
 FGameplayTag Attributes_Secondary_Armor;
 FGameplayTag Attributes_Secondary_ArmorPenetration;
 FGameplayTag Attributes_Secondary_BlockChance;
 FGameplayTag Attributes_Secondary_CriticalHitChance;
 FGameplayTag Attributes_Secondary_CriticalHitDamage;
 FGameplayTag Attributes_Secondary_CriticalHitResistance;
 FGameplayTag Attributes_Secondary_HealthRegeneration;
 FGameplayTag Attributes_Secondary_ManaRegeneration;
 FGameplayTag Attributes_Secondary_MaxHealth;
 FGameplayTag Attributes_Secondary_MaxMana;
 
protected:

private:
 static FBaseGameplayTags GameplayTags;
};
