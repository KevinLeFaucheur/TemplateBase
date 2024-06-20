// Retropsis @ 2024

#include "Equipment/Weapon/HitScanWeapon.h"
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

		if(UWorld* World = GetWorld())
		{
			FHitResult FireHit;
			World->LineTraceSingleByChannel(FireHit, Start, End, ECC_Visibility);
			FVector BeamEnd = End;
			if(FireHit.bBlockingHit)
			{
				BeamEnd = FireHit.ImpactPoint;
				ApplyDamage(InstigatorController, FireHit);
				PlayHitEffects(World, FireHit);
			}
			ShowTracer(SocketTransform, World, BeamEnd);
		}
	}
}

// TODO: Change this to GAS
void AHitScanWeapon::ApplyDamage(AController* InstigatorController, const FHitResult& FireHit)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(FireHit.GetActor());
	if(HasAuthority() && PlayerCharacter && InstigatorController)
	{
		UGameplayStatics::ApplyDamage(PlayerCharacter, Damage, InstigatorController, this, UDamageType::StaticClass());
	}
}

void AHitScanWeapon::PlayHitEffects(const UWorld* World, const FHitResult& FireHit) const
{
	if(ImpactParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(World, ImpactParticles, FireHit.ImpactPoint, FireHit.ImpactNormal.Rotation());
	}
	if(ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(World, ImpactSound, FireHit.ImpactPoint);
	}
}

void AHitScanWeapon::ShowTracer(const FTransform& SocketTransform, UWorld* World, const FVector& BeamEnd) const
{
	if(BeamParticles)
	{
		if(UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(World, BeamParticles, SocketTransform))
		{
			Beam->SetVectorParameter(FName("Target"), BeamEnd);
		}
	}
}
