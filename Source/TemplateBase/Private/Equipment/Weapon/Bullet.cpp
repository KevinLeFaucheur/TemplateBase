// Retropsis @ 2024


#include "Equipment/Weapon/Bullet.h"
#include "GameFramework/ProjectileMovementComponent.h"

ABullet::ABullet()
{
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->SetIsReplicated(true);
	ProjectileMovementComponent->InitialSpeed = InitialSpeed;
	ProjectileMovementComponent->MaxSpeed = InitialSpeed;
}

void ABullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Apply Damage Here
	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
}

void ABullet::BeginPlay()
{
	Super::BeginPlay();
}