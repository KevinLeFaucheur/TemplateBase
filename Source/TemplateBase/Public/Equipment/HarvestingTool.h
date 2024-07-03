// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "Equipment/Tool.h"
#include "HarvestingTool.generated.h"

/**
 * 
 */
UCLASS()
class TEMPLATEBASE_API AHarvestingTool : public ATool
{
	GENERATED_BODY()

public:
	AHarvestingTool();

	UPROPERTY(VisibleAnywhere, Category="01-Equipment")
	TObjectPtr<UStaticMeshComponent> Mesh;

	virtual void DetachToolFromComponent() override;
	virtual void OnEquipped() override;
	virtual void OnDropped() override;
	virtual void OnSecondary() override;
};
