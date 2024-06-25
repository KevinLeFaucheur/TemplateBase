// Retropsis @ 2024

#include "Equipment/Weapon/Projectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
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

void AProjectile::ApplyDamageEffects(AActor* OtherActor)
{
	if(UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
	{
		DamageEffectParams.DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;			
		DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
		UBaseAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
		
		// TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
	}
}

void AProjectile::ApplyRadialDamage()
{
	// if(!IsValidOverlap(OtherActor)) return;
	
	if(HasAuthority())
	{
		TArray<AActor*> OverlappingActors;
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(GetOwner());
		ActorsToIgnore.Add(this);
		UBaseAbilitySystemLibrary::GetLivePlayersWithinRadius(this, OverlappingActors, ActorsToIgnore, RadialDamageOuterRadius, GetActorLocation());
		UBaseAbilitySystemLibrary::SetIsRadialDamageEffectParam(DamageEffectParams, true, RadialDamageInnerRadius, RadialDamageOuterRadius, GetActorLocation());
		for (AActor* OverlapActor : OverlappingActors)
		{
			if(UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OverlapActor))
			{
				DamageEffectParams.DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
				DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
				UBaseAbilitySystemLibrary::SetTargetAbilitySystemComponent(DamageEffectParams, TargetASC);
				const FVector Direction = GetActorLocation() - OverlapActor->GetActorLocation();
				UBaseAbilitySystemLibrary::SetAirborneDirection(DamageEffectParams, Direction, 800.f);
				UBaseAbilitySystemLibrary::SetDeathImpulseDirection(DamageEffectParams, Direction, 800.f);
				UBaseAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
			}				
		}
	}
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	HitReact(OtherActor);
	Destroy();
}

void AProjectile::HitReact(AActor* OtherActor)
{
	if(IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(OtherActor))
	{
		PlayerInterface->PlayHitReactMontage();
	}
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

void AProjectile::SpawnTrailSystem()
{
	if(TrailSystem) TrailComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
		TrailSystem, GetRootComponent(), FName(), GetActorLocation(), GetActorRotation(),
		EAttachLocation::KeepWorldPosition, false);
}

void AProjectile::DestroyTimerStart()
{
	GetWorldTimerManager().SetTimer(DestroyTimer, this, &AProjectile::DestroyTimerEnd, DestroyTime);
}

void AProjectile::DestroyTimerEnd()
{
	Destroy();
}

void AProjectile::Destroyed()
{
	Super::Destroyed();
	PlayImpactEffects();
}

