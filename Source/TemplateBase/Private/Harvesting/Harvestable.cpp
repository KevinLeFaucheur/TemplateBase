// Retropsis @ 2024

#include "Harvesting/Harvestable.h"

AHarvestable::AHarvestable()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void AHarvestable::BeginPlay()
{
	Super::BeginPlay();
}