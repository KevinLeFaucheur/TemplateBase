// Retropsis @ 2024

#include "AbilitySystem/Actor/SkillProjectile.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/BaseAbilitySystemLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"

ASkillProjectile::ASkillProjectile()
{
	ProjectileEffect = CreateDefaultSubobject<UNiagaraComponent>("ProjectileEffect");
	ProjectileEffect->SetupAttachment(GetRootComponent());
}

void ASkillProjectile::BeginPlay()
{
	Super::BeginPlay();

	SpawnVFX(MuzzleEffect, EffectSpawnOffset);
}

void ASkillProjectile::SpawnVFX(UNiagaraSystem* EffectToSpawn, const FVector& SpawnOffset) const
{
	if(EffectToSpawn == nullptr) return;

	const FVector SpawnLocation = GetActorLocation() + SpawnOffset;
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, EffectToSpawn, SpawnLocation);
}

void ASkillProjectile::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!IsValidOverlap(OtherActor)) return;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("OnSphereBeginOverlap")));
	
	if(UBaseAbilitySystemLibrary::IsHostile(GetOwner(), OtherActor))
	{
		ProjectileMovementComponent->StopMovementImmediately();
		ProjectileEffect->SetVariableBool(FName("ShouldKillMainSlash"), true);
		SpawnVFX(ImpactEffect, EffectSpawnOffset);
		
		if(!bHit) OnHit();
		if(HasAuthority())
		{
			ApplyDamageEffects(OtherActor);
			Destroy();
		}
		else bHit = true;
	}
}
