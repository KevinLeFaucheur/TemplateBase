// Retropsis @ 2024

#include "AbilitySystem/Ability/FireBolt.h"
#include "BaseGameplayTags.h"
#include "AbilitySystem/BaseAbilitySystemLibrary.h"
#include "AbilitySystem/Actor/SpellProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

FString UFireBolt::GetDescription(int32 Level)
{
	// const int32 MinDamage = GetMinDamageByDamageType(Level, FBaseGameplayTags::Get().Damage_Fire);
	// const int32 MaxDamage = GetMaxDamageByDamageType(Level, FBaseGameplayTags::Get().Damage_Fire);
	const int32 MinDamage = DamageRange.DamageMin.GetValueAtLevel(Level);
	const int32 MaxDamage = DamageRange.DamageMax.GetValueAtLevel(Level);
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
	// const int32 MinDamage = GetMinDamageByDamageType(Level, FBaseGameplayTags::Get().Damage_Fire);
	// const int32 MaxDamage = GetMaxDamageByDamageType(Level, FBaseGameplayTags::Get().Damage_Fire);
	const int32 MinDamage = DamageRange.DamageMin.GetValueAtLevel(Level);
	const int32 MaxDamage = DamageRange.DamageMax.GetValueAtLevel(Level);
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

void UFireBolt::SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& CombatSocketTag,
	bool bOverridePitch, float PitchOverride, AActor* HomingTarget)
{
	if(!GetAvatarActorFromActorInfo()->HasAuthority()) return;

	// TODO: Data driven Socket
	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), CombatSocketTag);
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if(bOverridePitch) Rotation.Pitch = PitchOverride;

	const int32 EffectiveNumProjectiles = FMath::Min(NumProjectiles, GetAbilityLevel());	
	TArray<FRotator> Rotations = UBaseAbilitySystemLibrary::EvenlySpacedRotators(Rotation.Vector(), FVector::UpVector, ProjectileSpread, EffectiveNumProjectiles);
	for (const FRotator& Rot : Rotations)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rot.Quaternion());
		
		ASpellProjectile* Projectile = GetWorld()->SpawnActorDeferred<ASpellProjectile>(
			SpellProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetAvatarActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		// TODO: Decide what to do with a NULL HomingTarget (aiming at the sky)
		if(HomingTarget && HomingTarget->Implements<UCombatInterface>())
		{
			Projectile->ProjectileMovementComponent->HomingTargetComponent = HomingTarget->GetRootComponent();
		}
		else
		{
			Projectile->HomingTargetSceneComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
			Projectile->HomingTargetSceneComponent->SetWorldLocation(ProjectileTargetLocation);
			Projectile->ProjectileMovementComponent->HomingTargetComponent = Projectile->HomingTargetSceneComponent;
		}
		Projectile->ProjectileMovementComponent->HomingAccelerationMagnitude = FMath::FRandRange(HomingAccelerationMin, HomingAccelerationMax);
		Projectile->ProjectileMovementComponent->bIsHomingProjectile = bLaunchHomingProjectiles;
		Projectile->FinishSpawning(SpawnTransform);
	}
}
