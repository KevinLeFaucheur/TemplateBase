// Retropsis @ 2024


#include "AbilitySystem/Ability/Electrocute.h"

FString UElectrocute::GetDescription(int32 Level)
{
	const int32 MinDamage = DamageRange.DamageMin.GetValueAtLevel(Level);
	const int32 MaxDamage = DamageRange.DamageMax.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	if(Level == 1)
	{
		return FString::Printf(TEXT(
			"<Title>ELECTROCUTE</>\n\n"
			"<Small>Level: </><Level>%d</>\n"
			"<Small>MP: </><ManaCost>%.1f</>\n"
			"<Small>CD: </><Cooldown>%.1f</>\n\n"
			
			"<Default>Emits a lightning beam repeatedly dealing: </>"
			"<Damage>%d-%d</>"
			"<Default> lightning damage with a chance to stun.</>\n\n"
			), Level, ManaCost, Cooldown, MinDamage, MaxDamage);
	}
	else
	{
		return FString::Printf(TEXT(
			"<Title>ELECTROCUTE</>\n\n"
			"<Small>Level: </><Level>%d</>\n"
			"<Small>MP: </><ManaCost>%.1f</>\n"
			"<Small>CD: </><Cooldown>%.1f</>\n\n"
			
			"<Default>Emits a lightning beam, propagating to %d additional targets and repeatedly dealing: </>"
			"<Damage>%d-%d</>"
			"<Default> lightning damage with a chance to stun.</>\n\n"
			), Level, ManaCost, Cooldown, FMath::Min(Level, MaxNumHitTargets - 1), MinDamage, MaxDamage);
	}
}

FString UElectrocute::GetNextLevelDescription(int32 Level)
{
	const int32 MinDamage = DamageRange.DamageMin.GetValueAtLevel(Level);
	const int32 MaxDamage = DamageRange.DamageMax.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	
	return FString::Printf(TEXT(
		"<Title>NEXT LEVEL:</>\n\n"
		"<Small>Level: </><Level>%d</>\n"
		"<Small>MP: </><ManaCost>%.1f</>\n"
		"<Small>CD: </><Cooldown>%.1f</>\n\n"
			
		"<Default>Emits a lightning beam, propagating to %d additional targets and repeatedly dealing: </>"
		"<Damage>%d-%d</>"
		"<Default> lightning damage with a chance to stun.</>\n\n"
		), Level, ManaCost, Cooldown,  FMath::Min(Level, MaxNumHitTargets - 1), MinDamage, MaxDamage);
}
