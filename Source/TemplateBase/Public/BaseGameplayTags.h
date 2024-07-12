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
 
 /*
 * Resistances Attributes
 */
FGameplayTag Attributes_Resistance_Fire;
FGameplayTag Attributes_Resistance_Ice;
FGameplayTag Attributes_Resistance_Wind;
FGameplayTag Attributes_Resistance_Lightning;
FGameplayTag Attributes_Resistance_Holy;
FGameplayTag Attributes_Resistance_Dark;
FGameplayTag Attributes_Resistance_Physical;

 /*
  * Meta Attributes
  */
FGameplayTag Attributes_Meta_IncomingXP;
  	
 /*
  * Damage Types
  */
FGameplayTag Damage;
FGameplayTag Damage_Physical;
FGameplayTag Damage_Fire;
FGameplayTag Damage_Ice;
FGameplayTag Damage_Wind;
FGameplayTag Damage_Lightning;
FGameplayTag Damage_Holy;
FGameplayTag Damage_Dark;

TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;
   	  	
 /*
  * Status Effects
  */
FGameplayTag StatusEffect_Burn;
FGameplayTag StatusEffect_Stun;
FGameplayTag StatusEffect_Poison;
FGameplayTag StatusEffect_Sleep;
FGameplayTag StatusEffect_Silence;
 
FGameplayTag StatusEffect_Chance;
FGameplayTag StatusEffect_Damage;
FGameplayTag StatusEffect_Duration;
FGameplayTag StatusEffect_Frequency;

TMap<FGameplayTag, FGameplayTag> DamageTypesToStatusEffects;
 
 /*
  * Abilities
  */
FGameplayTag Abilities_None;
FGameplayTag Abilities_Attack;
FGameplayTag Abilities_Summon;
FGameplayTag Abilities_HitReact;
FGameplayTag Abilities_Fire_FireBolt;
FGameplayTag Abilities_Fire_FireBlast;
FGameplayTag Abilities_Lightning_Electrocute;
FGameplayTag Abilities_Arcane_ArcaneShards;

 FGameplayTag Abilities_Passive_Barrier;
 FGameplayTag Abilities_Passive_HealthDrain;
 FGameplayTag Abilities_Passive_ManaDrain;

FGameplayTag Abilities_Status_Locked;
FGameplayTag Abilities_Status_Eligible;
FGameplayTag Abilities_Status_Unlocked;
FGameplayTag Abilities_Status_Equipped;
 
FGameplayTag Abilities_Type_Offensive;
FGameplayTag Abilities_Type_Passive;
FGameplayTag Abilities_Type_None;
 
FGameplayTag Abilities_Weapon_Sword_1;
FGameplayTag Abilities_Weapon_Sword_2;
 
FGameplayTag Abilities_Monk_Basic;
 
FGameplayTag Abilities_Throw_1;
FGameplayTag Abilities_Throw_2;

 /*
  * Gameplay Cues
  */
FGameplayTag GameplayCue_FireBlast;

 /*
  * Abilities Cooldowns
  */
FGameplayTag Cooldown_Fire_FireBolt;
 
/*
 * Side Effects
 */
FGameplayTag Effects_HitReact;
 
 /*
  * Combat Sockets
  */
FGameplayTag CombatSocket_Weapon;
FGameplayTag CombatSocket_LeftHand;
FGameplayTag CombatSocket_RightHand;
FGameplayTag CombatSocket_Fang;
FGameplayTag CombatSocket_Tail;
FGameplayTag CombatSocket_LeftFeet;
FGameplayTag CombatSocket_RightFeet;
  
 /*
  * Montages
  */
FGameplayTag Montage_Attack_1;
FGameplayTag Montage_Attack_2;
FGameplayTag Montage_Attack_3;
FGameplayTag Montage_Attack_4;
 
 /*
  * Input Tags
  */
FGameplayTag InputTag_LMB;
FGameplayTag InputTag_RMB;
FGameplayTag InputTag_1;
FGameplayTag InputTag_2;
FGameplayTag InputTag_3;
FGameplayTag InputTag_4;
FGameplayTag InputTag_5;
FGameplayTag InputTag_6;
FGameplayTag InputTag_7;
FGameplayTag InputTag_8;
FGameplayTag InputTag_9;
FGameplayTag InputTag_0;
FGameplayTag InputTag_Passive_1;
FGameplayTag InputTag_Passive_2;
FGameplayTag InputTag_Passive_3;
FGameplayTag InputTag_Passive_4;
FGameplayTag InputTag_Passive_5;
 
FGameplayTag InputTag_Throw_1;
FGameplayTag InputTag_Throw_2;
 
 /*
 * Player Tags
 */
FGameplayTag Player_Block_InputPressed;
FGameplayTag Player_Block_InputHeld;
FGameplayTag Player_Block_InputReleased;
FGameplayTag Player_Block_CursorTrace;
 
protected:

private:
 static FBaseGameplayTags GameplayTags;
};
