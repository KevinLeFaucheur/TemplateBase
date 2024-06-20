// Retropsis @ 2024

#include "Equipment/Weapon/Projectile.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/BaseAbilitySystemLibrary.h"
#include "Components/BoxComponent.h"
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

bool AProjectile::IsValidOverlap(AActor* OtherActor)
{
	if(!IsValid(DamageEffectParams.SourceAbilitySystemComponent)) return false;
	AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	if(SourceAvatarActor == OtherActor) return false;
	if(!UBaseAbilitySystemLibrary::IsHostile(SourceAvatarActor, OtherActor)) return false;

	return true;
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(OtherActor))
	{
		PlayerInterface->PlayHitReactMontage();
	}
	Destroy();
}

void AProjectile::PlayImpactEffects()
{
	if(ImpactParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, GetActorTransform());
	}
	if(ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	}
}

void AProjectile::Destroyed()
{
	Super::Destroyed();
	PlayImpactEffects();
}

