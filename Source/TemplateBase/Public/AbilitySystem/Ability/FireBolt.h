// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/ProjectileSpell.h"
#include "FireBolt.generated.h"

/**
 * 
 */
UCLASS()
class TEMPLATEBASE_API UFireBolt : public UProjectileSpell
{
	GENERATED_BODY()

public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
};
