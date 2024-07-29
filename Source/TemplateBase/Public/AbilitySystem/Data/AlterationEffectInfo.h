// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AlterationEffectInfo.generated.h"

/**
 * 
*/
USTRUCT(BlueprintType)
struct FAlterationEffect
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(Categories="AlterationEffect"))
	FGameplayTag AlterationEffectTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Name = FText();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Description = FString();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Icon = nullptr;
};

UCLASS()
class TEMPLATEBASE_API UAlterationEffectInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GAS|Abilities")
	TArray<FAlterationEffect> AlterationEffectInformation;

	FAlterationEffect FindAlterationEffectForTag(const FGameplayTag& AlterationEffectTag, bool bLogNotFound = false) const;
	
};
