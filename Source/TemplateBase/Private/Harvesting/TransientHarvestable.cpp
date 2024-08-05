// Retropsis @ 2024

#include "Harvesting/TransientHarvestable.h"

#include "Net/UnrealNetwork.h"

ATransientHarvestable::ATransientHarvestable()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void ATransientHarvestable::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATransientHarvestable, Direction);
}

void ATransientHarvestable::BeginPlay()
{
	Super::BeginPlay();
}

