// Retropsis @ 2024

#include "AbilitySystem/Ability/ProjectileSpell.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Actor/SpellProjectile.h"
#include "Interaction/CombatInterface.h"


void UProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& CombatSocketTag, bool bOverridePitch, float PitchOverride)
{
	if(!GetAvatarActorFromActorInfo()->HasAuthority()) return;

	// TODO: Data driven Socket
	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), CombatSocketTag);
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if(bOverridePitch)
	{
		Rotation.Pitch = PitchOverride; // Parallel to the ground if needed
	}

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SocketLocation);
	SpawnTransform.SetRotation(Rotation.Quaternion());
		
	ASpellProjectile* Projectile = GetWorld()->SpawnActorDeferred<ASpellProjectile>(
		SpellProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetAvatarActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
	Projectile->FinishSpawning(SpawnTransform);
}

void UProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,  const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}