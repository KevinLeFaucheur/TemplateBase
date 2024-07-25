// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OverheadWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class TEMPLATEBASE_API UOverheadWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void ShowPlayerNetRole(const APawn* InPawn);
	
	UFUNCTION(BlueprintCallable)
	void ShowPlayerName(const ACharacter* InCharacter);
	
protected:
	virtual void NativeDestruct() override;
	void SetDisplayText(const FString& TextToDisplay) const;

public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> DisplayText;
};
