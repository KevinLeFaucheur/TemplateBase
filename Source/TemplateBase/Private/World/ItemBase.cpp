// Retropsis @ 2024

#include "World/ItemBase.h"

AItemBase::AItemBase()
{
	PrimaryActorTick.bCanEverTick = false;
	SetReplicates(true);
}

void AItemBase::BeginPlay()
{
	Super::BeginPlay();
}

