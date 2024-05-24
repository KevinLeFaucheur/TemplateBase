// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TEMPLATEBASE_API IPlayerInterface
{
	GENERATED_BODY()

public:
	virtual void PlayHitReactMontage() {};

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
};
