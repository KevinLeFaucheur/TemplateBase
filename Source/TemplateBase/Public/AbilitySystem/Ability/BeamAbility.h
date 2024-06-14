// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/DamageGameplayAbility.h"
#include "BeamAbility.generated.h"

/**
 * 
 */
UCLASS()
class TEMPLATEBASE_API UBeamAbility : public UDamageGameplayAbility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void StoreMouseDataInfo(const FHitResult& HitResult);

	UFUNCTION(BlueprintCallable)
	void StoreOwnerVariables();
	
	UFUNCTION(BlueprintCallable)
	void TraceFirstTarget(const FVector& BeamTargetLocation);
	
protected:
	UPROPERTY(BlueprintReadWrite, Category="Ability|Beam")
	FVector MouseHitLocation;

	UPROPERTY(BlueprintReadWrite, Category="Ability|Beam")
	TObjectPtr<AActor> MouseHitActor;

	UPROPERTY(BlueprintReadWrite, Category="Ability|Beam")
	TObjectPtr<APlayerController> OwnerPlayerController;
	
	UPROPERTY(BlueprintReadWrite, Category="Ability|Beam")
	TObjectPtr<ACharacter> OwnerCharacter;
private:
};
