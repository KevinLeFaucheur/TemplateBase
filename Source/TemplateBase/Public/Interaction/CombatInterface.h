// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TEMPLATEBASE_API ICombatInterface
{
	GENERATED_BODY()

public:
	virtual int32 GetCharacterLevel();

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
};
