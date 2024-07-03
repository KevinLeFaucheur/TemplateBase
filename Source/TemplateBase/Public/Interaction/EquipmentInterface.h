// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "Equipment/EquipmentData.h"
#include "UObject/Interface.h"
#include "EquipmentInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEquipmentInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TEMPLATEBASE_API IEquipmentInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FEquipmentInfo GetEquipmentInfo();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UseItem(AActor* PlayerCharacter);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void MontageEnd();

	
};
