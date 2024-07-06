// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Harvestable.generated.h"

class UHarvestableInfo;

UCLASS()
class TEMPLATEBASE_API AHarvestable : public AActor
{
	GENERATED_BODY()
	
public:	
	AHarvestable();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UHarvestableInfo> Resources = nullptr;

public:	

};
