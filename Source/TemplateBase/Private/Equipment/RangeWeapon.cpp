// Retropsis @ 2024

#include "Equipment/RangeWeapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Equipment/EquipmentComponent.h"
#include "Equipment/Weapon/Casing.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Player/PlayerCharacter.h"
#include "Player/PlayerCharacterController.h"

ARangeWeapon::ARangeWeapon()
{
}

void ARangeWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ARangeWeapon, Ammunition);
}

void ARangeWeapon::Activate(const FVector& HitTarget)
{
	Super::Activate(HitTarget);
	SpendAmmunition();
	EjectCasing();
}

/*
 * Ammunition
 */
void ARangeWeapon::SpendAmmunition()
{
	Ammunition = FMath::Clamp(Ammunition - 1, 0, AmmunitionCapacity);
	SetHUDAmmunition();
}

void ARangeWeapon::OnRep_Ammunition()
{
	// TODO: Maybe some interface call instead
	OwnerCharacter = OwnerCharacter == nullptr ? OwnerCharacter = Cast<APlayerCharacter>(GetOwner()) : OwnerCharacter;
	if(OwnerCharacter && OwnerCharacter->EquipmentComponent && IsFull())
	{
		OwnerCharacter->EquipmentComponent->JumpToReloadEnd();
	}
	SetHUDAmmunition();
}

void ARangeWeapon::EjectCasing()
{
	if(CasingClass)
	{
		if(const USkeletalMeshSocket* AmmoEjectSocket = Mesh->GetSocketByName(FName("AmmoEject")))
		{
			const FTransform SocketTransform = AmmoEjectSocket->GetSocketTransform(Mesh);
			if(UWorld* World = GetWorld())
			{
				World->SpawnActor<ACasing>(CasingClass, SocketTransform.GetLocation(), SocketTransform.GetRotation().Rotator());
			}
		}
	}
}

void ARangeWeapon::SetHUDAmmunition()
{
	OwnerCharacter = OwnerCharacter == nullptr ? OwnerCharacter = Cast<APlayerCharacter>(GetOwner()) : OwnerCharacter;
	if(OwnerCharacter)
	{
		OwnerController = OwnerController == nullptr ? Cast<APlayerCharacterController>(OwnerCharacter->Controller) : OwnerController;
		if(OwnerController)
		{
			OwnerController->SetHUDAmmunition(Ammunition);
		}
	}
}

void ARangeWeapon::AddAmmunition(int32 AmountToAdd)
{
	Ammunition = FMath::Clamp(Ammunition + AmountToAdd, 0, AmmunitionCapacity);
	SetHUDAmmunition();
}

/*
 * Scatter
 */
FVector ARangeWeapon::TraceEndWithScatter(const FVector& TraceStart, const FVector& HitTarget) const
{
	const FVector ToTargetNormalized = (HitTarget - TraceStart).GetSafeNormal();
	const FVector SphereCenter = TraceStart + ToTargetNormalized * DistanceToSphere;
	const FVector RandVector = UKismetMathLibrary::RandomUnitVector() * FMath::FRandRange(0.f, SphereRadius);
	const FVector EndLocation = SphereCenter + RandVector;
	const FVector ToEndLocation = EndLocation - TraceStart;

	if (bDebugScatter)
	{
		UKismetSystemLibrary::DrawDebugSphere(this, SphereCenter, SphereRadius, 12, FLinearColor::Red, 3.f);
		UKismetSystemLibrary::DrawDebugSphere(this, EndLocation, 4.f, 6, FLinearColor::White, 5.f);
		UKismetSystemLibrary::DrawDebugLine(this, TraceStart, TraceStart + ToEndLocation * TRACE_LENGTH / ToEndLocation.Size(), FLinearColor::Green, 5.f);
	}
	return FVector(TraceStart + ToEndLocation * TRACE_LENGTH / ToEndLocation.Size());
}
