// Retropsis @ 2024


#include "AbilitySystem/Ability/ArcaneShards.h"

FString UArcaneShards::GetDescription(int32 Level)
{
	const int32 MinDamage = DamageRange.DamageMin.GetValueAtLevel(Level);
	const int32 MaxDamage = DamageRange.DamageMax.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	if(Level == 1)
	{
		return FString::Printf(TEXT(
			"<Title>ARCANE SHARDS: </>"
			"<Small>Level: </><Level>%d</><Default> - </>"
			"<Small>MP: </><ManaCost>%.1f</><Default> - </>"
			"<Small>CD: </><Cooldown>%.1f</>\n"
			
			"<Default>Summon a shard of arcane energy dealing: </>"
			"<Damage>%d-%d</>"
			"<Default> radial arcane damage.</>\n\n"
			), Level, ManaCost, Cooldown, MinDamage, MaxDamage);
	}
	else
	{
		return FString::Printf(TEXT(
			"<Title>ARCANE SHARDS: </>"
			"<Small>Level: </><Level>%d</><Default> - </>"
			"<Small>MP: </><ManaCost>%.1f</><Default> - </>"
			"<Small>CD: </><Cooldown>%.1f</>\n"
			
			"<Default>Summon %d shards of arcane energy: </>"
			"<Damage>%d-%d</>"
			"<Default> radial arcane damage.</>\n\n"
			), Level, ManaCost, Cooldown, FMath::Min(Level, MaxNumShards), MinDamage, MaxDamage);
	}
}

FString UArcaneShards::GetNextLevelDescription(int32 Level)
{
	const int32 MinDamage = DamageRange.DamageMin.GetValueAtLevel(Level);
	const int32 MaxDamage = DamageRange.DamageMax.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	
	return FString::Printf(TEXT(
		"<Title>NEXT LEVEL: </>"
		"<Small>Level: </><Level>%d</><Default> - </>"
		"<Small>MP: </><ManaCost>%.1f</><Default> - </>"
		"<Small>CD: </><Cooldown>%.1f</>\n"
			
		"<Default>Summon %d shards of arcane energy: </>"
		"<Damage>%d-%d</>"
		"<Default> radial arcane damage.</>\n\n"
		), Level, ManaCost, Cooldown, FMath::Min(Level, MaxNumShards), MinDamage, MaxDamage);
}

