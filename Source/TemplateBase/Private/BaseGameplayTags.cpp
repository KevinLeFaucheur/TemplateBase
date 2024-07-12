// Retropsis @ 2024
// Retropsis @ 2024

#include "BaseGameplayTags.h"
#include "GameplayTagsManager.h"

FBaseGameplayTags FBaseGameplayTags::GameplayTags;

void FBaseGameplayTags::InitializeNativeGameplayTags()
{
	/*
	 * Vital Attributes
	*/
	
	/*
	 * Primary Attributes
	 */
	GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Strength"), FString("Primary factor for physical attacks"));
	GameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Intelligence"), FString("Primary factor for magical attacks"));
	GameplayTags.Attributes_Primary_Vitality = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Vitality"), FString("Primary factor for armor and health"));
	GameplayTags.Attributes_Primary_Spirit = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Spirit"), FString("Primary factor for mana and magical resistance"));
	GameplayTags.Attributes_Primary_Dexterity = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Dexterity"), FString("Primary factor for accuracy"));
	GameplayTags.Attributes_Primary_Luck = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Luck"), FString("Random luck"));
	
	/*
	 * Secondary Attributes
	 */
	GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Armor"), FString("Reduces damage taken, improves Armor"));
	GameplayTags.Attributes_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.ArmorPenetration"), FString("Ignores percentage of enemy's armor, chance for critical hit"));
	GameplayTags.Attributes_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.BlockChance"), FString("Chance to reduce receiving damage"));
	GameplayTags.Attributes_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitChance"), FString("Chance for a critical hit"));
	GameplayTags.Attributes_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitDamage"), FString("Critical hit damage amount"));
	GameplayTags.Attributes_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitResistance"), FString("Reduces damage taken from critical hits"));
	GameplayTags.Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.HealthRegeneration"), FString("Amount of health healed every 1 second"));
	GameplayTags.Attributes_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.ManaRegeneration"), FString("Amount of mana healed every 1 second"));
	GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxHealth"), FString("Max amount of health"));
	GameplayTags.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxMana"), FString("Max amount of mana"));

	/*
	 * Resistance Attributes
	 */
	GameplayTags.Attributes_Resistance_Physical= UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Physical"), FString("Resistance to Physical Damage"));
	GameplayTags.Attributes_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Fire"), FString("Resistance to Fire Damage"));
	GameplayTags.Attributes_Resistance_Ice = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Ice"), FString("Resistance to Ice Damage"));
	GameplayTags.Attributes_Resistance_Wind = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Wind"), FString("Resistance to Wind Damage"));
	GameplayTags.Attributes_Resistance_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Lightning"), FString("Resistance to Lightning Damage"));
	GameplayTags.Attributes_Resistance_Holy= UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Holy"), FString("Resistance to Holy Damage"));
	GameplayTags.Attributes_Resistance_Dark = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Dark"), FString("Resistance to Dark Damage"));
	
	/*
	 * Meta Attributes
	 */
	GameplayTags.Attributes_Meta_IncomingXP = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Meta.IncomingXP"), FString("Meta Attribute for Incoming XP"));
	
	/*
	 * Damage Types
	 */
	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage"), FString("Damage"));
	GameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Physical"), FString("Physical Damage"));
	GameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Fire"), FString("Fire Damage"));
	GameplayTags.Damage_Ice = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Ice"), FString("Ice Damage"));
	GameplayTags.Damage_Wind = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Wind"), FString("Wind Damage"));
	GameplayTags.Damage_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Lightning"), FString("Lightning Damage"));
	GameplayTags.Damage_Holy = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Holy"), FString("Holy Damage"));
	GameplayTags.Damage_Dark = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Dark"), FString("Dark Damage"));

	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Physical, GameplayTags.Attributes_Resistance_Physical);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Fire, GameplayTags.Attributes_Resistance_Fire);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Ice, GameplayTags.Attributes_Resistance_Ice);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Wind, GameplayTags.Attributes_Resistance_Wind);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Lightning, GameplayTags.Attributes_Resistance_Lightning);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Holy, GameplayTags.Attributes_Resistance_Holy);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Dark, GameplayTags.Attributes_Resistance_Dark);
   	  	
	/*
	 * Status Effects
	 */
	GameplayTags.StatusEffect_Burn = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("StatusEffect.Burn"), FString("Status Effect from Fire"));
	GameplayTags.StatusEffect_Stun = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("StatusEffect.Stun"), FString("Status Effect from Ice"));
	GameplayTags.StatusEffect_Sleep = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("StatusEffect.Sleep"), FString("Status Effect"));
	GameplayTags.StatusEffect_Poison = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("StatusEffect.Poison"), FString("Status Effect"));
	GameplayTags.StatusEffect_Silence = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("StatusEffect.Silence"), FString("Status Effect"));

	GameplayTags.StatusEffect_Chance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("StatusEffect.Chance"), FString("Status Effect Chance"));
	GameplayTags.StatusEffect_Duration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("StatusEffect.Duration"), FString("Status Effect Duration"));
	GameplayTags.StatusEffect_Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("StatusEffect.Damage"), FString("Status Effect Damage"));
	GameplayTags.StatusEffect_Frequency = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("StatusEffect.Frequency"), FString("Status Effect Frequency"));
	
	// TODO: Should work differently
	GameplayTags.DamageTypesToStatusEffects.Add(GameplayTags.Damage_Fire, GameplayTags.StatusEffect_Burn);
	GameplayTags.DamageTypesToStatusEffects.Add(GameplayTags.Damage_Lightning, GameplayTags.StatusEffect_Stun);
	GameplayTags.DamageTypesToStatusEffects.Add(GameplayTags.Damage_Ice, GameplayTags.StatusEffect_Silence);
	GameplayTags.DamageTypesToStatusEffects.Add(GameplayTags.Damage_Physical, GameplayTags.StatusEffect_Poison);
	GameplayTags.DamageTypesToStatusEffects.Add(GameplayTags.Damage_Wind, GameplayTags.StatusEffect_Sleep);
	
	/*
	 * Abilities
	 */
	GameplayTags.Abilities_None = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.None"), FString("None"));
	GameplayTags.Abilities_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Attack"), FString("Melee Attack"));
	GameplayTags.Abilities_Summon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Summon"), FString("Ability Summon"));
	
	GameplayTags.Abilities_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Fire.FireBolt"), FString("Fire Element Ability FireBolt"));
	GameplayTags.Abilities_Fire_FireBlast = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Fire.FireBlast"), FString("Fire Element Ability FireBlast"));
	GameplayTags.Abilities_Lightning_Electrocute = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Lightning.Electrocute"), FString("Lightning Element Electrocute Ability"));
	GameplayTags.Abilities_Arcane_ArcaneShards = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Arcane.ArcaneShards"), FString("Arcane Element Shards Ability"));
	
	GameplayTags.Abilities_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.HitReact"), FString("HitReact Ability Effect"));
	
	GameplayTags.Abilities_Passive_Barrier = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Passive.Barrier"), FString("Passive Ability Barrier"));
	GameplayTags.Abilities_Passive_HealthDrain = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Passive.HealthDrain"), FString("Passive Ability Health Drain"));
	GameplayTags.Abilities_Passive_ManaDrain = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Passive.ManaDrain"), FString("Passive Ability Mana Drain"));
	
	GameplayTags.Abilities_Status_Locked = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Locked"), FString("Abilities Status Locked"));
	GameplayTags.Abilities_Status_Eligible = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Eligible"), FString("Abilities Status Eligible"));
	GameplayTags.Abilities_Status_Unlocked = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Unlocked"), FString("Abilities Status Unlocked"));
	GameplayTags.Abilities_Status_Equipped = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Equipped"), FString("Abilities Status Equipped"));
	
	GameplayTags.Abilities_Type_None = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Type.None"), FString("Abilities Type None"));
	GameplayTags.Abilities_Type_Offensive = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Type.Offensive"), FString("Abilities Type Offensive"));
	GameplayTags.Abilities_Type_Passive = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Type.Passive"), FString("Abilities Type Passive"));
	
	GameplayTags.Abilities_Weapon_Sword_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Weapon.Sword.1"), FString("Abilities Weapon Sword 1"));
	GameplayTags.Abilities_Weapon_Sword_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Weapon.Sword.2"), FString("Abilities Weapon Sword 2"));
	
	GameplayTags.Abilities_Monk_Basic = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Monk.Basic"), FString("Abilities Monk Basic"));
	
	GameplayTags.Abilities_Throw_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Throw.1"), FString("Abilities Throw 1"));
	GameplayTags.Abilities_Throw_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Throw.2"), FString("Abilities Throw 2"));

	/*
	 * Gameplay Cues
	 */
	GameplayTags.GameplayCue_FireBlast = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.FireBlast"), FString("Gameplay Cue FireBlast"));
	
	/*
	 * Abilities Cooldown
	 */
	GameplayTags.Cooldown_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Fire.FireBolt"), FString("Fire Elemental Ability FireBolt"));
	
	/*
	 * Side Effects
	 */
	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effects.HitReact"), FString("Hit Reacting Effect"));
		
	/*
	 * Combat Sockets
	 */
	GameplayTags.CombatSocket_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.Weapon"), FString("Attack with Weapon Socket"));
	GameplayTags.CombatSocket_LeftHand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.LeftHand"), FString("Attack with Left Hand Socket"));
	GameplayTags.CombatSocket_RightHand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.RightHand"), FString("Attack with Right Hand Socket"));
	GameplayTags.CombatSocket_Fang = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.Fang"), FString("Attack with Fang Socket"));
	GameplayTags.CombatSocket_Tail = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.Tail"), FString("Attack with Tail Socket"));
	GameplayTags.CombatSocket_LeftFeet = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.LeftFeet"), FString("Attack with Left Feet Socket"));
	GameplayTags.CombatSocket_RightFeet = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.RightFeet"), FString("Attack with Right Feet Socket"));

	/*
	 * Montages
	 */
	GameplayTags.Montage_Attack_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.1"), FString("Montage Attack 1"));
	GameplayTags.Montage_Attack_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.2"), FString("Montage Attack 2"));
	GameplayTags.Montage_Attack_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.3"), FString("Montage Attack 3"));
	GameplayTags.Montage_Attack_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.4"), FString("Montage Attack 4"));
	
	/*
	 * Input Tags
	 */
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.LMB"), FString("Left Mouse Button"));
	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.RMB"), FString("Right Mouse Button"));
	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.1"), FString("1"));
	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.2"), FString("2"));
	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.3"), FString("3"));
	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.4"), FString("4"));
	GameplayTags.InputTag_5 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.5"), FString("5"));
	GameplayTags.InputTag_6 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.6"), FString("6"));
	GameplayTags.InputTag_7 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.7"), FString("7"));
	GameplayTags.InputTag_8 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.8"), FString("8"));
	GameplayTags.InputTag_9 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.9"), FString("9"));
	GameplayTags.InputTag_0 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.0"), FString("0"));
	GameplayTags.InputTag_Passive_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Passive.1"), FString("Passive 1"));
	GameplayTags.InputTag_Passive_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Passive.2"), FString("Passive 2"));
	GameplayTags.InputTag_Passive_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Passive.3"), FString("Passive 3"));
	GameplayTags.InputTag_Passive_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Passive.4"), FString("Passive 4"));
	GameplayTags.InputTag_Passive_5 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Passive.5"), FString("Passive 5"));
	
	GameplayTags.InputTag_Throw_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Throw.1"), FString("Throw 1"));
	GameplayTags.InputTag_Throw_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Throw.2"), FString("Throw 2"));

	/*
	* Player Tags
	*/
	GameplayTags.Player_Block_InputPressed = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Player.Block.InputPressed"), FString("Block input pressed from player"));
	GameplayTags.Player_Block_InputHeld = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Player.Block.InputHeld"), FString("Block input pressed from player"));
	GameplayTags.Player_Block_InputReleased = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Player.Block.InputReleased"), FString("Block input pressed from player"));
	GameplayTags.Player_Block_CursorTrace = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Player.Block.CursorTrace"), FString("Block trace under cursor"));
}
