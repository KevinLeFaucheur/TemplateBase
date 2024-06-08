// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "Character/CharacterData.h"
#include "PlayerCharacter.generated.h"

class UNiagaraComponent;
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
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void OnRep_ReplicatedMovement() override;
	virtual void Tick(float DeltaTime) override;
	
	void SetOverlappingTool(ATool* Tool);

	virtual void Jump() override;
	void CalculateAO_Pitch();
	float CalculateSpeed();
	void CrouchButtonPressed();
	void FireButtonPressed();
	void FireButtonReleased();
	void AimButtonPressed();
	void AimButtonReleased();
	void EquipButtonPressed();

	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed();
	
	//~ Combat Interface
	virtual int32 GetCharacterLevel_Implementation() override;
	//~ Combat Interface

	void PlayFireMontage(bool bAiming);
	
	//~ Player Interface
	virtual void PlayHitReactMontage() override;
	virtual int32 GetXP_Implementation() const override;
	virtual int32 GetAttributePointReward_Implementation(int32 Level) const override;
	virtual int32 GetSpellPointReward_Implementation(int32 Level) const override;
	virtual int32 GetAttributePoints_Implementation() const override;
	virtual int32 GetSpellPoints_Implementation() const override;
	virtual int32 FindLevelForXP_Implementation(int32 InXP) const override;
	virtual void AddToXP_Implementation(int32 InXP) override;
	virtual void AddToAttributePoints_Implementation(int32 InAttributePoints) override;
	virtual void AddToPlayerLevel_Implementation(int32 InPlayerLevel) override;
	virtual void AddToSpellPoints_Implementation(int32 InSpellPoints) override;
	virtual void LevelUp_Implementation() override;
	//~ Player Interface

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastHitReact();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<UEquipmentComponent> EquipmentComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> OverheadWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="PlayerCharacter|Effects")
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;

protected:
	void AimOffset(float DeltaTime);
	void SimProxiesTurn();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUp() const;
	
private:
	virtual void InitAbilityActorInfo() override;
	
	UPROPERTY(ReplicatedUsing=OnRep_OverlappingTool)
	TObjectPtr<ATool> OverlappingTool;

	UFUNCTION()
	void OnRep_OverlappingTool(ATool* LastTool);

	/* Aiming & Animation */
	float AO_Yaw;
	float InterpAO_Yaw;
	float AO_Pitch;
	FRotator StartingAimRotation;

	void TurnInPlace(float DeltaTime);
	ETurningInPlace TurningInPlace = ETurningInPlace::ETIP_NotTurning;

	UPROPERTY(EditAnywhere, Category="PlayerCharacter")
	float AngleBeforeTurning = 25.f;

	bool bRotateRootBone;
	float TurnThreshold = 0.5f;
	FRotator ProxyRotationLastFrame;
	FRotator ProxyRotation;
	float ProxyYaw;
	float TimeSinceLastMovementReplication;

	/* Camera */
	void HideCharacterIfCameraClose();

	UPROPERTY(EditAnywhere, Category="PlayerCharacter|Camera")
	float CameraThreshold = 200.f;

public:
	ATool* GetEquippedTool();
	FVector GetHitTarget() const;
	bool IsEquipped();
	bool IsAiming();
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return SpringArm; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; }
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; }
	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return TurningInPlace; }
	FORCEINLINE bool ShouldRotateRootBone() const { return bRotateRootBone; }
	
};
