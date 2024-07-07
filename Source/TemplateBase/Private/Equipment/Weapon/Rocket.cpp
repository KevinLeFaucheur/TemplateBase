// Retropsis @ 2024

#include "Equipment/Weapon/Rocket.h"
#include "NiagaraComponent.h"
#include "NiagaraSystemInstanceController.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"
#include "Equipment/Weapon/RocketMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ARocket::ARocket()
{
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>("RocketMesh");
	ProjectileMesh->SetupAttachment(GetRootComponent());
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	RocketMovementComponent = CreateDefaultSubobject<URocketMovementComponent>("RocketMovementComponent");
	RocketMovementComponent->bRotationFollowsVelocity = true;
	RocketMovementComponent->SetIsReplicated(true);
	RocketMovementComponent->InitialSpeed = InitialSpeed;
	RocketMovementComponent->MaxSpeed = InitialSpeed;
	
}

void ARocket::SpawnTrailSound()
{
	if(TrailSound && TrailSoundAttenuation)
	{
		TrailSoundComponent = UGameplayStatics::SpawnSoundAttached(
			TrailSound, GetRootComponent(), FName(), GetActorLocation(), EAttachLocation::KeepWorldPosition, false,
			1.f, 1.f, 0.f, TrailSoundAttenuation, nullptr, false);
	}
}

void ARocket::BeginPlay()
{
	Super::BeginPlay();
	
	if(!HasAuthority())
	{
		HitCollision->OnComponentHit.AddDynamic(this, &ARocket::OnHit);
	}
	SpawnTrailSystem();
	SpawnTrailSound();
}

void ARocket::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(OtherActor == GetOwner()) return;
	ApplyRadialDamage();
	
	// Super::OnHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);

	DestroyTimerStart();
	PlayImpactEffects();
	if(ProjectileMesh) ProjectileMesh->SetVisibility(false);
	if(HitCollision) HitCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if(TrailComponent && TrailComponent->GetSystemInstanceController()) TrailComponent->GetSystemInstanceController()->Deactivate();
	if(TrailSoundComponent && TrailSoundComponent->IsPlaying()) TrailSoundComponent->Stop();
}

void ARocket::Destroyed() {}