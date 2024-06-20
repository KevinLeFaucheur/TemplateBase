// Retropsis @ 2024

#include "Equipment/Weapon/MultiHitScanWeapon.h"
#include "Engine/SkeletalMeshSocket.h"

void AMultiHitScanWeapon::Activate(const FVector& HitTarget)
{
	const APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if(OwnerPawn == nullptr) return;
	AController* InstigatorController = OwnerPawn->GetController();
	
	const USkeletalMeshSocket* MuzzleFlashSocket = GetMesh()->GetSocketByName(FName("MuzzleFlash"));
	if(MuzzleFlashSocket && InstigatorController)
	{
		const FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetMesh());
		const FVector Start = SocketTransform.GetLocation();
		// const FVector End = Start + (HitTarget - Start) * 1.25;
		const FVector End = TraceEndWithScatter(Start, HitTarget);

		

		// if(UWorld* World = GetWorld())
		// {
		// 	FHitResult FireHit;
		// 	World->LineTraceSingleByChannel(FireHit, Start, End, ECC_Visibility);
		// 	FVector BeamEnd = End;
		// 	if(FireHit.bBlockingHit)
		// 	{
		// 		BeamEnd = FireHit.ImpactPoint;
		// 		ApplyDamage(InstigatorController, FireHit);
		// 		PlayHitEffects(World, FireHit);
		// 	}
		// 	ShowTracer(SocketTransform, World, BeamEnd);
		// }
	}
}
