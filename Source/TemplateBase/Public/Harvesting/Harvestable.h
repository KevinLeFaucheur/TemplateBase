// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/HarvestableInterface.h"
#include "Harvestable.generated.h"

class UHarvestableInfo;

UCLASS()
class TEMPLATEBASE_API AHarvestable : public AActor, public IHarvestableInterface
{
	GENERATED_BODY()
	
public:	
	AHarvestable();

	//~ Harvestable Interface
	virtual float GetHealth_Implementation() override { return Health; }
	virtual void UpdateHealth_Implementation(const float NewHealth) override { Health = NewHealth; }
	virtual TSoftObjectPtr<UHarvestableInfo> GetResources_Implementation() override { return Resources; }
	//~ Harvestable Interface

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UHarvestableInfo> Resources = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="Harvesting")
	float Health = 50.f;

public:	

};
