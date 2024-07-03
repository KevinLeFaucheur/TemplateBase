// Retropsis @ 2024

#include "AbilitySystem/Ability/ThrowAbility.h"
#include "Equipment/Weapon/Projectile.h"

void UThrowAbility::SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& CombatSocketTag, bool bOverridePitch, float PitchOverride)
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
		
	AProjectile* Projectile = GetWorld()->SpawnActorDeferred<AProjectile>(
		ThrowableClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetAvatarActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
	Projectile->FinishSpawning(SpawnTransform);
}

void UThrowAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,  const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

bool UThrowAbility::CanThrow() const
{
	if(GetAvatarActorFromActorInfo()->Implements<UCombatInterface>())
	{
		return ICombatInterface::Execute_GetThrowableCount(GetAvatarActorFromActorInfo()) > 0;
	}
	return false;
}

void UThrowAbility::UseAvailableThrowable() const
{
	if(GetAvatarActorFromActorInfo()->Implements<UCombatInterface>())
	{
		ICombatInterface::Execute_SpendAvailableThrowable(GetAvatarActorFromActorInfo());
	}
}
