// Retropsis @ 2024

#include "AbilitySystem/Ability/SummonAbility.h"
#include "Kismet/KismetSystemLibrary.h"

TArray<FVector> USummonAbility::GetSpawnLocations()
{
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	const float DeltaSpread = SpawnSpread / (NumMinions + 1);
	TArray<FVector> SpawnLocations;

	const FVector LeftOfSpread = Forward.RotateAngleAxis(- SpawnSpread / 2.f, FVector::UpVector);
	const FVector RightOfSpread = Forward.RotateAngleAxis(SpawnSpread / 2.f, FVector::UpVector);
	for (int i = 0; i < NumMinions; ++i)
	{
		const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * (i + 1), FVector::UpVector);
		// UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Location, Location + Direction * MaxSpawnDistance, 2.f, FLinearColor::White, 3.f);

		FVector ChosenSpawnLocation = Location + Direction * FMath::FRandRange(MinSpawnDistance, MaxSpawnDistance);
		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(Hit, ChosenSpawnLocation + FVector(0.f, 0.f, 400.f), ChosenSpawnLocation - FVector(0.f, 0.f, 400.f), ECC_Visibility);
		if(Hit.bBlockingHit)
		{
			ChosenSpawnLocation = Hit.ImpactPoint;
		}
		
		SpawnLocations.Add(ChosenSpawnLocation);
		// UKismetSystemLibrary::DrawDebugSphere(GetWorld(), ChosenSpawnLocation, 15.f, 12, FLinearColor::Black, 3.f);
	}

	// UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Location, Location + LeftOfSpread * MaxSpawnDistance, 2.f, FLinearColor::Green, 3.f);
	// UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Location, Location + RightOfSpread * MaxSpawnDistance, 2.f, FLinearColor::Green, 3.f);
	// UKismetSystemLibrary::DrawDebugSphere(GetWorld(), Location + LeftOfSpread * MinSpawnDistance, 15.f, 12, FLinearColor::Green, 3.f);
	// UKismetSystemLibrary::DrawDebugSphere(GetWorld(), Location + RightOfSpread * MinSpawnDistance, 15.f, 12, FLinearColor::Green, 3.f);
	// UKismetSystemLibrary::DrawDebugSphere(GetWorld(), Location + LeftOfSpread * MaxSpawnDistance, 15.f, 12, FLinearColor::Green, 3.f);
	// UKismetSystemLibrary::DrawDebugSphere(GetWorld(), Location + RightOfSpread * MaxSpawnDistance, 15.f, 12, FLinearColor::Green, 3.f);

	return SpawnLocations;
}

TSubclassOf<APawn> USummonAbility::GetRandomMinionClass()
{
	return MinionClasses[FMath::RandRange(0, MinionClasses.Num() - 1)];
}
