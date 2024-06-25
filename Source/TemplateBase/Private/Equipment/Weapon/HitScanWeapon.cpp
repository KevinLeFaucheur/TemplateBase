// Retropsis @ 2024

#include "Equipment/Weapon/HitScanWeapon.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "BaseGameplayTags.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Player/PlayerCharacter.h"


void AHitScanWeapon::Activate(const FVector& HitTarget)
{
	Super::Activate(HitTarget);

	const APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if(OwnerPawn == nullptr) return;
	AController* InstigatorController = OwnerPawn->GetController();
	
	const USkeletalMeshSocket* MuzzleFlashSocket = GetMesh()->GetSocketByName(FName("MuzzleFlash"));
	if(MuzzleFlashSocket && InstigatorController)
	{
		const FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetMesh());
		const FVector Start = SocketTransform.GetLocation();
		const FVector End = Start + (HitTarget - Start) * 1.25;

		FHitResult FireHit;
		WeaponTraceHit(Start, HitTarget, FireHit);
		ApplyDamage(FireHit);
		PlayHitEffects(FireHit);
	}
}

void AHitScanWeapon::WeaponTraceHit(const FVector& TraceStart, const FVector& HitTarget, FHitResult& OutHit) const
{
	if(const UWorld* World = GetWorld())
	{
		const FVector End = bUseScatter ? TraceEndWithScatter(TraceStart, HitTarget) :  TraceStart + (HitTarget - TraceStart) * 1.25;
		World->LineTraceSingleByChannel(OutHit, TraceStart, End, ECC_Visibility);
		FVector BeamEnd = End;
		if(OutHit.bBlockingHit)
		{
			BeamEnd = OutHit.ImpactPoint;
		}
		ShowTracer(TraceStart, World, BeamEnd);
	}
}

// TODO: Investigate for refactor
void AHitScanWeapon::ApplyDamage(const FHitResult& FireHit)
{
	if(HasAuthority() && FireHit.GetActor())
	{
		ApplyHitScanDamage(FireHit.GetActor());
	}
}

void AHitScanWeapon::PlayHitEffects(const FHitResult& FireHit, bool bOverrideMultipliers) const
{
	if(const UWorld* World = GetWorld())
	{
		if(ImpactParticles)
		{
			UGameplayStatics::SpawnEmitterAtLocation(World, ImpactParticles, FireHit.ImpactPoint, FireHit.ImpactNormal.Rotation());
		}
		if(ImpactSound)
		{
			if(bOverrideMultipliers) UGameplayStatics::PlaySoundAtLocation(World, ImpactSound, FireHit.ImpactPoint, .5f, FMath::FRandRange(-.5f, .5f));
			else UGameplayStatics::PlaySoundAtLocation(World, ImpactSound, FireHit.ImpactPoint);
		}
	}
}

void AHitScanWeapon::ShowTracer(const FVector& AtLocation, const UWorld* World, const FVector& BeamEnd) const
{
	if(BeamParticles)
	{
		if(UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(World, BeamParticles, AtLocation, FRotator::ZeroRotator, true))
		{
			Beam->SetVectorParameter(FName("Target"), BeamEnd);
		}
	}
}
