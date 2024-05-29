// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "BaseAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class TEMPLATEBASE_API UBaseAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static UBaseAssetManager& Get();

protected:
	virtual void StartInitialLoading() override;
	
};
