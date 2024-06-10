// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "UI/Controller/BaseWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

/**
 * 
 */
UCLASS()
class TEMPLATEBASE_API USpellMenuWidgetController : public UBaseWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
};
