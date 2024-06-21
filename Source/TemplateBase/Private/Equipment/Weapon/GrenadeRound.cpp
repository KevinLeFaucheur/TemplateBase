// Retropsis @ 2024

#include "Equipment/Weapon/GrenadeRound.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AGrenadeRound::AGrenadeRound()
{
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>("GrenadeRound");
	ProjectileMesh->SetupAttachment(GetRootComponent());
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->SetIsReplicated(true);
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->InitialSpeed = InitialSpeed;
	ProjectileMovementComponent->MaxSpeed = InitialSpeed;
}

void AGrenadeRound::BeginPlay()
{
	AActor::BeginPlay();
	SpawnTrailSystem();
	DestroyTimerStart();
	ProjectileMovementComponent->OnProjectileBounce.AddDynamic(this, &AGrenadeRound::OnBounce);
}

void AGrenadeRound::OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
	if(BouncingSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, BouncingSound, GetActorLocation());
	}
}

void AGrenadeRound::Destroyed()
{
	// Apply Radial Damage
	ApplyRadialDamage();	
	Super::Destroyed();
}
