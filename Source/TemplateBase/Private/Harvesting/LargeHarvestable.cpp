// Retropsis @ 2024


#include "Harvesting/LargeHarvestable.h"

ALargeHarvestable::ALargeHarvestable()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);
}
