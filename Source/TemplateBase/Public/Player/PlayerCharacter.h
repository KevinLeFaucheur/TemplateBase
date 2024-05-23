// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Character/BaseCharacter.h"
#include "Character/CharacterData.h"
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
	virtual void Tick(float DeltaTime) override;
	void SetOverlappingTool(ATool* Tool);
	
	virtual void Jump() override;
	void CrouchButtonPressed();
	void FireButtonPressed();
	void FireButtonReleased();
	void AimButtonPressed();
	void AimButtonReleased();
	void EquipButtonPressed();

	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed();

	void PlayFireMontage(bool bAiming);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<UEquipmentComponent> EquipmentComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> OverheadWidget;

protected:
	void AimOffset(float DeltaTime);
	
private:
	UPROPERTY(ReplicatedUsing=OnRep_OverlappingTool)
	TObjectPtr<ATool> OverlappingTool;

	UFUNCTION()
	void OnRep_OverlappingTool(ATool* LastTool);

	float AO_Yaw;
	float InterpAO_Yaw;
	float AO_Pitch;
	FRotator StartingAimRotation;

	void TurnInPlace(float DeltaTime);
	ETurningInPlace TurningInPlace = ETurningInPlace::ETIP_NotTurning;

	UPROPERTY(EditAnywhere, Category="PlayerCharacter")
	float AngleBeforeTurning = 25.f;

public:
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return SpringArm; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	ATool* GetEquippedTool();
	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; }
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; }
	bool IsEquipped();
	bool IsAiming();
	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return TurningInPlace; }
};
