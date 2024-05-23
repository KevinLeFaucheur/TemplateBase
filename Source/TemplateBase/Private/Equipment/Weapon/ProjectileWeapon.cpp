// Retropsis @ 2024

#include "Equipment/Weapon/ProjectileWeapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Equipment/Weapon/Projectile.h"

void AProjectileWeapon::Activate(const FVector& HitTarget)
{
	Super::Activate(HitTarget);

	if(!HasAuthority()) return;;
	
	APawn* InstigatorPawn = Cast<APawn>(GetOwner());
	if(const USkeletalMeshSocket* MuzzleFlashSocket = GetMesh()->GetSocketByName(FName("MuzzleFlash")))
	{
		const FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetMesh());
		const FVector ToTarget = HitTarget - SocketTransform.GetLocation();
		const FRotator TargetRotation = ToTarget.Rotation();
		if(ProjectileClass && InstigatorPawn)
		{
			if(UWorld* World = GetWorld())
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = GetOwner();
				SpawnParams.Instigator = InstigatorPawn;
				World->SpawnActor<AProjectile>(ProjectileClass, SocketTransform.GetLocation(), TargetRotation, SpawnParams);
			}
		}
	}
}
