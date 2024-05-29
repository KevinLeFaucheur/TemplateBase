// Retropsis @ 2024

#include "BaseAssetManager.h"
#include "BaseGameplayTags.h"

UBaseAssetManager& UBaseAssetManager::Get()
{
	check(GEngine);
	return *Cast<UBaseAssetManager>(GEngine->AssetManager);
}

void UBaseAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	FBaseGameplayTags::InitializeNativeGameplayTags();
}
