// Retropsis @ 2024

#include "BaseAssetManager.h"
#include "BaseGameplayTags.h"
// #include "AbilitySystemGlobals.h"

UBaseAssetManager& UBaseAssetManager::Get()
{
	check(GEngine);
	return *Cast<UBaseAssetManager>(GEngine->AssetManager);
}

void UBaseAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	FBaseGameplayTags::InitializeNativeGameplayTags();
	// Not anymore needed after 5.3
	// UAbilitySystemGlobals::Get().InitGlobalData();
}
