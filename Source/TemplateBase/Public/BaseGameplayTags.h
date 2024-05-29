// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 *  Single for Native Gameplay tags
 */
struct  FBaseGameplayTags
{
public:
static const FBaseGameplayTags& Get() { return GameplayTags; }
static  void InitializeNativeGameplayTags();
 
protected:

private:
 static FBaseGameplayTags GameplayTags;
};
