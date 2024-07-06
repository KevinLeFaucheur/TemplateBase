// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "BaseWidgetController.generated.h"

class UAbilityInfo;
class UBaseAttributeSet;
class UBaseAbilitySystemComponent;
class APlayerCharacterState;
class APlayerCharacterController;
class UAttributeSet;
class UAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChangedSignature, int32, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoSignature, const FBaseAbilityInfo&, Info);

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() {}

	FWidgetControllerParams(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
	: PlayerController(PC), PlayerState(PS), AbilitySystemComponent(ASC), AttributeSet(AS) {}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
};

/**
 * 
 */
UCLASS()
class TEMPLATEBASE_API UBaseWidgetController : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);
	
	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues() {}
	virtual void BindCallbacksToDependencies() {}
	void BroadcastAbilityInfo();

	UPROPERTY(BlueprintAssignable, Category="GAS|Abilities")
	FAbilityInfoSignature AbilityInfoDelegate;
	
protected:
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerController> PlayerController;
	
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerState> PlayerState;
	
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;
	
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerCharacterController> PlayerCharacterController;
	
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerCharacterState> PlayerCharacterState;
	
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UBaseAbilitySystemComponent> BaseAbilitySystemComponent;
	
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UBaseAttributeSet> BaseAttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GAS|WidgetData")
	TObjectPtr<UAbilityInfo> AbilityInfo;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GAS|WidgetData")
	TObjectPtr<UAbilityInfo> ToolAbilityInfo;

	APlayerCharacterController* GetPlayerCharacterController();
	APlayerCharacterState* GetPlayerCharacterState();
	UBaseAbilitySystemComponent* GetBaseAbilitySystemComponent();
	UBaseAttributeSet* GetBaseAttributeSet();
};
