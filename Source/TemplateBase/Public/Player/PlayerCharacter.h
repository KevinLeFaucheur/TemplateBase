// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Character/BaseCharacter.h"
#include "PlayerCharacter.generated.h"

class UEquipmentComponent;
class ATool;
class UWidgetComponent;
class UCameraComponent;
class USpringArmComponent;
/**
 * 
 */
UCLASS()
class TEMPLATEBASE_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;
	void SetOverlappingTool(ATool* Tool);
	
	void EquipButtonPressed();
	void CrouchButtonPressed();

	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<UEquipmentComponent> EquipmentComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> OverheadWidget;

private:
	UPROPERTY(ReplicatedUsing=OnRep_OverlappingTool)
	TObjectPtr<ATool> OverlappingTool;

	UFUNCTION()
	void OnRep_OverlappingTool(ATool* LastTool);

public:
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return SpringArm; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	bool IsEquipped();
};
