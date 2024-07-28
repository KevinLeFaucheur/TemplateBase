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
	UPROPERTY(BlueprintReadWrite)
	FVector MouseHitLocation = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite)
	AActor* MouseHitActor = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category="Abilities|Input", meta=(Categories="InputTag"))
	FGameplayTag StartupInputTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Abilities")
	UAnimMontage* SpellCastingMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Abilities", meta=(Categories="GameplayCue"))
	FGameplayTag GameplayCueTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Abilities", meta=(Categories="Event.Montage"))
	FGameplayTag EventMontageTag;
	
	virtual FString GetDescription(int32 Level);
	virtual FString GetNextLevelDescription(int32 Level);
	static FString GetLockedDescription(int32 Level);

protected:
	float GetManaCost(float InLevel = 1.f) const;
	float GetCooldown(float InLevel = 1.f) const;
};
