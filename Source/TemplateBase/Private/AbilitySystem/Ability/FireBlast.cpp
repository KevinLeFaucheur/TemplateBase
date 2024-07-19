// Retropsis @ 2024

#include "AbilitySystem/Ability/FireBlast.h"
#include "AbilitySystem/BaseAbilitySystemLibrary.h"
#include "AbilitySystem/Actor/FireBall.h"

FString UFireBlast::GetDescription(int32 Level)
{
	const int32 MinDamage = DamageRange.DamageMin.GetValueAtLevel(Level);
	const int32 MaxDamage = DamageRange.DamageMax.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	
	return FString::Printf(TEXT(
			"<Title>FIRE BLAST: </>"
			"<Small>Level: </><Level>%d</><Default> - </>"
			"<Small>MP: </><ManaCost>%.1f</><Default> - </>"
			"<Small>CD: </><Cooldown>%.1f</>\n"
			
			"<Default>Launches a lightning beam, propagating to %d additional targets and dealing: </>"
			"<Damage>%d-%d</>"
			"<Default>radial fire damage with a chance to stun.</>\n\n"
			), Level, ManaCost, Cooldown, FMath::Min(Level + 2, NumFireballs), MinDamage, MaxDamage);
}

FString UFireBlast::GetNextLevelDescription(int32 Level)
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
			
			"<Default>Launches a lightning beam, propagating to %d additional targets and dealing: </>"
			"<Damage>%d-%d</>"
			"<Default>radial fire damage with a chance to stun.</>\n\n"
			), Level, ManaCost, Cooldown, FMath::Min(Level + 2, NumFireballs), MinDamage, MaxDamage);
}

TArray<AFireBall*> UFireBlast::SpawnFireBalls()
{
	TArray<AFireBall*> FireBalls; 
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	TArray<FRotator> Rotators = UBaseAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, 360.f, NumFireballs);

	for (const FRotator& Rotator : Rotators)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(Location);
		SpawnTransform.SetRotation(Rotator.Quaternion());
		AFireBall* FireBall = GetWorld()->SpawnActorDeferred<AFireBall>(
			FireBallClass, SpawnTransform,
			GetOwningActorFromActorInfo(), CurrentActorInfo->PlayerController->GetPawn(),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		FireBall->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		FireBall->ReturnToActor = GetAvatarActorFromActorInfo();
		FireBall->RadialDamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		FireBall->SetOwner(GetAvatarActorFromActorInfo());
		FireBalls.Add(FireBall);
		FireBall->FinishSpawning(SpawnTransform);
	}
	return FireBalls;
}
