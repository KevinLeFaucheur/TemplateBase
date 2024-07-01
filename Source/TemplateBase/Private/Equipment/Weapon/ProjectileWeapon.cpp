// Retropsis @ 2024

#include "Equipment/Weapon/ProjectileWeapon.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "BaseGameplayTags.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Equipment/Weapon/Projectile.h"
#include "Kismet/KismetSystemLibrary.h"

void AProjectileWeapon::Activate(const FVector& HitTarget)
{
	Super::Activate(HitTarget);

	if(!HasAuthority()) return;;
	
	APawn* InstigatorPawn = Cast<APawn>(GetOwner());
	if(const USkeletalMeshSocket* MuzzleFlashSocket = GetMesh()->GetSocketByName(FName("MuzzleFlash")))
	{
		const FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetMesh());
		const FVector ToTarget = HitTarget - SocketTransform.GetLocation();
		const FRotator TargetRotation = ToTarget.Rotation();
		if(ProjectileClass && InstigatorPawn)
		{
			if(UWorld* World = GetWorld())
			{
				FTransform SpawnTransform;
				SpawnTransform.SetLocation(SocketTransform.GetLocation());
				SpawnTransform.SetRotation(TargetRotation.Quaternion());
				
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = GetOwner();
				SpawnParams.Instigator = InstigatorPawn;
				AProjectile* Projectile = World->SpawnActorDeferred<AProjectile>(ProjectileClass, SocketTransform, GetOwner(), InstigatorPawn, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
				
				// AProjectile* Projectile = World->SpawnActor<AProjectile>(ProjectileClass, SocketTransform.GetLocation(), TargetRotation, SpawnParams);

				// const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
				// const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, 1.0f, SourceASC->MakeEffectContext());
				//
				// const FBaseGameplayTags GameplayTags = FBaseGameplayTags::Get();
				//
				// const float ScaledMagnitudeMin = DamageRange.DamageMin.GetValueAtLevel(1.f);
				// const float ScaledMagnitudeMax = DamageRange.DamageMax.GetValueAtLevel(1.f);
				// const float Magnitude = FMath::FRandRange(ScaledMagnitudeMin, ScaledMagnitudeMax);
				// UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageType, Magnitude);
				
				Projectile->DamageEffectSpecHandle = /*SpecHandle*/ MakeDamageEffectSpec();
				Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
				Projectile->FinishSpawning(SpawnTransform);
			}
		}
	}
}
