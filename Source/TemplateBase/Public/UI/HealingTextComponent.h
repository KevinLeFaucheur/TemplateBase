// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealingTextComponent.generated.h"

/**
 * 
 */
UCLASS()
class TEMPLATEBASE_API UHealingTextComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetHealingText(float Healing, bool bCriticalHit);

	UPROPERTY(BlueprintReadOnly)
	float Delay = 0.f;
};
