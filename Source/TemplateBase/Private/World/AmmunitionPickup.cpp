// Retropsis @ 2024


#include "World/AmmunitionPickup.h"

#include "Interaction/PlayerInterface.h"

void AAmmunitionPickup::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if(OtherActor->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_PickupAmmunition(OtherActor, AmmunitionType, AmmunitionAmount);
	}
	Destroy();
}
