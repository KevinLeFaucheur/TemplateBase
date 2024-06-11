// Retropsis @ 2024

#include "AbilitySystem/Ability/FireBolt.h"
#include "BaseGameplayTags.h"

FString UFireBolt::GetDescription(int32 Level)
{
	const int32 MinDamage = GetMinDamageByDamageType(Level, FBaseGameplayTags::Get().Damage_Fire);
	const int32 MaxDamage = GetMaxDamageByDamageType(Level, FBaseGameplayTags::Get().Damage_Fire);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	if(Level == 1)
	{
		return FString::Printf(TEXT(
			"<Title>FIRE BOLT</>\n\n"
			"<Small>Level: </><Level>%d</>\n"
			"<Small>MP: </><ManaCost>%.1f</>\n"
			"<Small>CD: </><Cooldown>%.1f</>\n\n"
			
			"<Default>Launches a bolt of fire, exploding on impact and dealing: </>"
			"<Damage>%d-%d</>"
			"<Default> fire damage with a chance to burn.</>\n\n"
			), Level, ManaCost, Cooldown, MinDamage, MaxDamage);
	}
	else
	{
		return FString::Printf(TEXT(
			"<Title>FIRE BOLT</>\n\n"
			"<Small>Level: </><Level>%d</>\n"
			"<Small>MP: </><ManaCost>%.1f</>\n"
			"<Small>CD: </><Cooldown>%.1f</>\n\n"
			
			"<Default>Launches %d bolts of fire, exploding on impact and dealing: </>"
			"<Damage>%d-%d</>"
			"<Default> fire damage with a chance to burn.</>\n\n"
			), Level, ManaCost, Cooldown, FMath::Min(Level, NumProjectiles), MinDamage, MaxDamage);
	}
}

FString UFireBolt::GetNextLevelDescription(int32 Level)
{
	const int32 MinDamage = GetMinDamageByDamageType(Level, FBaseGameplayTags::Get().Damage_Fire);
	const int32 MaxDamage = GetMaxDamageByDamageType(Level, FBaseGameplayTags::Get().Damage_Fire);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	
	return FString::Printf(TEXT(
		"<Title>NEXT LEVEL:</>\n\n"
		"<Small>Level: </><Level>%d</>\n"
		"<Small>MP: </><ManaCost>%.1f</>\n"
		"<Small>CD: </><Cooldown>%.1f</>\n\n"
			
		"<Default>Launches %d bolts of fire, exploding on impact and dealing: </>"
		"<Damage>%d-%d</>"
		"<Default> fire damage with a chance to burn.</>\n\n"
		), Level, ManaCost, Cooldown, FMath::Min(Level, NumProjectiles), MinDamage, MaxDamage);
}