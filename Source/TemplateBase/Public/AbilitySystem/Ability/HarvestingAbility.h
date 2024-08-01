// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/BaseGameplayAbility.h"
#include "Data/HarvestableInfo.h"
#include "Equipment/EquipmentData.h"
#include "HarvestingAbility.generated.h"

/**
 * 
 */
UCLASS()
class TEMPLATEBASE_API UHarvestingAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void TryHarvest(const FRotator Rotation);
	
	UFUNCTION(Server, Reliable)
	void ServerOnOverlap(const FRotator Rotation);
	
protected:
	virtual void Harvest(float Damage, AActor* HitActor);
	virtual void CalculateResources(const TArray<FResource>& Resources, float Damage);
	
	UPROPERTY(EditAnywhere, Category="Equipment")
	float HarvestingDistance = 200.f;
	
	UPROPERTY(EditAnywhere, Category="Equipment")
	float HarvestingRadius = 20.f;
	
	FHarvestingToolInfo HarvestingToolInfo;
};
