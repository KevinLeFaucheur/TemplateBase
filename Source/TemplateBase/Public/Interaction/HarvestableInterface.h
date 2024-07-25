// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "Data/HarvestableInfo.h"
#include "UObject/Interface.h"
#include "HarvestableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHarvestableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TEMPLATEBASE_API IHarvestableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	float GetHealth();

	UFUNCTION(BlueprintNativeEvent)
	void UpdateHealth(float NewHealth);
	
	UFUNCTION(BlueprintNativeEvent)
	TSoftObjectPtr<UHarvestableInfo> GetResources();
};
