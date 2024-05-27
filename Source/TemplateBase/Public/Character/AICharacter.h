// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "AICharacter.generated.h"

/**
 * 
 */
UCLASS()
class TEMPLATEBASE_API AAICharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AAICharacter();

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
};
