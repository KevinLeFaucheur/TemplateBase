// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "UI/Controller/BaseWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

/**
 * 
 */
UCLASS()
class TEMPLATEBASE_API UAttributeMenuWidgetController : public UBaseWidgetController
{
	GENERATED_BODY()

public:
	virtual void BindCallbacksToDependencies() override;
	virtual void BroadcastInitialValues() override;
};
