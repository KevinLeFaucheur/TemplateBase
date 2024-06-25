// Retropsis @ 2024

#include "AbilitySystem/Actor/SpellProjectile.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/BaseAbilitySystemLibrary.h"
#include "Components/AudioComponent.h"
#include "TemplateBase/TemplateBase.h"

ASpellProjectile::ASpellProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetCollisionObjectType(ECC_Projectile);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	SetRootComponent(Sphere);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	ProjectileMovementComponent->InitialSpeed = 550.f;
	ProjectileMovementComponent->MaxSpeed = 550.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
}

void ASpellProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeSPan);
	SetReplicateMovement(true);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ASpellProjectile::OnSphereBeginOverlap);

	if(LoopingSoundComponent)
	{
		LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());
		LoopingSoundComponent->bStopWhenOwnerDestroyed = true;
	}
}

void ASpellProjectile::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!IsValidOverlap(OtherActor)) return;
	if(!bHit) OnHit();
	
	if(HasAuthority())
	{
		ApplyDamageEffects(OtherActor);
		Destroy();
	}
	else bHit = true;
}

void ASpellProjectile::OnHit()
{
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
	bHit = true;
}

bool ASpellProjectile::IsValidOverlap(AActor* OtherActor)
{
	if(!IsValid(DamageEffectParams.SourceAbilitySystemComponent)) return false;
	AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	if(SourceAvatarActor == OtherActor) return false;
	if(!UBaseAbilitySystemLibrary::IsHostile(SourceAvatarActor, OtherActor)) return false;

	return true;
}

void ASpellProjectile::Destroyed()
{
	if(!bHit && !HasAuthority()) OnHit();
	Super::Destroyed();
}

void ASpellProjectile::ApplyDamageEffects(AActor* OtherActor)
{
	if(UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
	{
		DamageEffectParams.DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
			
		const bool bAirborne = FMath::RandRange(1, 100) < DamageEffectParams.AirborneChance;
		if(bAirborne)
		{
			FRotator Rotation = GetActorRotation();
			Rotation.Pitch = 45.f;
			const FVector AirborneDirection = Rotation.Vector();
			DamageEffectParams.AirborneForce = AirborneDirection * DamageEffectParams.AirborneForceMagnitude;
		}
			
		DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
		UBaseAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
	}
}
