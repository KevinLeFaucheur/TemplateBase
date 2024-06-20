// Retropsis @ 2024

#include "Equipment/Weapon/Rocket.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraSystemInstanceController.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"
#include "Equipment/Weapon/RocketMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/BaseAbilitySystemLibrary.h"

ARocket::ARocket()
{
	RocketMesh = CreateDefaultSubobject<UStaticMeshComponent>("RocketMesh");
	RocketMesh->SetupAttachment(GetRootComponent());
	RocketMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	RocketMovementComponent = CreateDefaultSubobject<URocketMovementComponent>("RocketMovementComponent");
	RocketMovementComponent->bRotationFollowsVelocity = true;
	RocketMovementComponent->SetIsReplicated(true);
	RocketMovementComponent->InitialSpeed = InitialSpeed;
	RocketMovementComponent->MaxSpeed = InitialSpeed;
	
}

void ARocket::BeginPlay()
{
	Super::BeginPlay();
	
	if(!HasAuthority())
	{
		HitCollision->OnComponentHit.AddDynamic(this, &ARocket::OnHit);
	}

	if(TrailSystem) TrailComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
		TrailSystem, GetRootComponent(), FName(), GetActorLocation(), GetActorRotation(),
		EAttachLocation::KeepWorldPosition, false);

	if(TrailSound && TrailSoundAttenuation)
	{
		TrailSoundComponent = UGameplayStatics::SpawnSoundAttached(
			TrailSound, GetRootComponent(), FName(), GetActorLocation(), EAttachLocation::KeepWorldPosition, false,
			1.f, 1.f, 0.f, TrailSoundAttenuation, nullptr, false);
	}
}

void ARocket::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(OtherActor == GetOwner())
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit Self"));
		return;;
	}
	// if(!IsValidOverlap(OtherActor)) return;
	//
	// if(HasAuthority())
	// {
	// 	if(UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
	// 	{
	// 		DamageEffectParams.DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
	// 		DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
	// 		UBaseAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
	// 	}
	// }
	
	// Super::OnHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);

	GetWorldTimerManager().SetTimer(DestroyTimer, this, &ARocket::DestroyTimerEnd, DestroyTime);
	PlayImpactEffects();
	if(RocketMesh) RocketMesh->SetVisibility(false);
	if(HitCollision) HitCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if(TrailComponent && TrailComponent->GetSystemInstanceController()) TrailComponent->GetSystemInstanceController()->Deactivate();
	if(TrailSoundComponent && TrailSoundComponent->IsPlaying()) TrailSoundComponent->Stop();
}

void ARocket::DestroyTimerEnd()
{
	Destroy();
}

void ARocket::Destroyed() {}