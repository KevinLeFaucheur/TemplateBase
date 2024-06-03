// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "BaseGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class TEMPLATEBASE_API UBaseGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()


public:
	UPROPERTY(EditDefaultsOnly, Category="GAS|Input")
	FGameplayTag StartupInputTag;
};
