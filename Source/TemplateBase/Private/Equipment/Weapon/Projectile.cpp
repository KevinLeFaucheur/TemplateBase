// Retropsis @ 2024

#include "Equipment/Weapon/Projectile.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"
#include "TemplateBase/TemplateBase.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	HitCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("HitCollision"));
	SetRootComponent(HitCollision);
	HitCollision->SetCollisionObjectType(ECC_WorldDynamic);
	HitCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	HitCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	HitCollision->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	HitCollision->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	HitCollision->SetCollisionResponseToChannel(ECC_SkeletalMesh, ECR_Block);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->InitialSpeed = 15000.f;
	ProjectileMovementComponent->MaxSpeed = 15000.f;
	// ProjectileMovementComponent->ProjectileGravityScale = 1.f;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	if(Tracer)
	{
		TracerComponent = UGameplayStatics::SpawnEmitterAttached(
			Tracer, HitCollision, FName(), GetActorLocation(), GetActorRotation(), EAttachLocation::KeepWorldPosition);
	}
	if(HasAuthority())
	{
		HitCollision->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	}
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if(IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(OtherActor))
	{
		PlayerInterface->PlayHitReactMontage();
	}
	Destroy();
}

void AProjectile::Destroyed()
{
	Super::Destroyed();
	
	if(ImpactParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, GetActorTransform());
	}
	if(ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	}
}

