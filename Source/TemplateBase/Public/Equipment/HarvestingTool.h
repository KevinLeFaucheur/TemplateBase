// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "Equipment/Tool.h"
#include "Harvesting/HarvestableData.h"
#include "HarvestingTool.generated.h"

struct FResource;
/**
 * 
 */
UCLASS()
class TEMPLATEBASE_API AHarvestingTool : public ATool
{
	GENERATED_BODY()

public:
	AHarvestingTool();

	UPROPERTY(VisibleAnywhere, Category="Equipment")
	TObjectPtr<UStaticMeshComponent> Mesh;

	//~ Equipment Interface
	// virtual void UseItem_Implementation(AActor* PlayerCharacter) override;
	virtual void OnHarvestingNotify_Implementation() override;
	virtual FHarvestingToolInfo GetHarvestingToolInfo_Implementation() override;
	//~ Equipment Interface

	UFUNCTION(Client, Reliable)
	void ClientGetViewRotation();
	
	UFUNCTION(Server, Reliable)
	void ServerOnOverlap(FRotator Rotation);

	virtual void DetachToolFromComponent() override;
	virtual void OnEquipped() override;
	virtual void OnDropped() override;
	virtual void OnSecondary() override;

protected:
	virtual void Harvest(float Damage, AActor* HitActor);
	virtual void CalculateResources(const TArray<FResource>& Resources, float Damage);

	UPROPERTY(EditAnywhere, Category="Equipment")
	EHarvestingToolType HarvestingToolType = EHarvestingToolType::Hatchet;

	UPROPERTY(EditAnywhere, Category="Equipment")
	EHarvestingToolTier HarvestingToolTier = EHarvestingToolTier::Stone;
	
	UPROPERTY(EditAnywhere, Category="Equipment")
	float HarvestingDamage = 12.f;
	
	UPROPERTY(EditAnywhere, Category="Equipment")
	float HarvestingDistance = 200.f;
	
	UPROPERTY(EditAnywhere, Category="Equipment")
	float HarvestingRadius = 20.f;
};
