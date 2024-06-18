// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/DamageGameplayAbility.h"
#include "FireBlast.generated.h"

class AFireBall;
/**
 * 
 */
UCLASS()
class TEMPLATEBASE_API UFireBlast : public UDamageGameplayAbility
{
	GENERATED_BODY()
	
public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;

	UFUNCTION(BlueprintCallable)
	TArray<AFireBall*> SpawnFireBalls();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 NumFireballs = 12;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AFireBall> FireBallClass;
};
