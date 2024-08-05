// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "Harvesting/TransientHarvestable.h"
#include "TransientHarvestableTree.generated.h"

class UCapsuleComponent;
/**
 * 
 */
UCLASS()
class TEMPLATEBASE_API ATransientHarvestableTree : public ATransientHarvestable
{
	GENERATED_BODY()

public:
	ATransientHarvestableTree();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCapsuleComponent> TrunkCapsule;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> TreeMesh;

protected:
	virtual void BeginPlay() override;
	void AddForce();

	UPROPERTY(EditDefaultsOnly, Category="Harvestable|Physics")
	float Force = 1500.f;
};
