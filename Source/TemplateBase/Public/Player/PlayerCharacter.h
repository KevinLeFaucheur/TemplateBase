// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "Character/CharacterData.h"
#include "Equipment/Tool.h"
#include "PlayerCharacter.generated.h"

class UHotbarComponent;
class UPlayerInventoryComponent;
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
	void ReloadButtonPressed();
	void ThrowButtonPressed();

	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed();
	
	UFUNCTION(Server, Reliable)
	void ServerLeftMouseButtonPressed();
	
	//~ Combat Interface
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) override;
	virtual int32 GetCharacterLevel_Implementation() override;
	virtual int32 GetThrowableCount_Implementation() override;
	virtual void SpendAvailableThrowable_Implementation() override;
	//~ Combat Interface

	void PlayFireMontage(bool bAiming);
	void PlayThrowMontage();
	void PlaySwapToolMontage();
	void PlayReloadMontage(const EToolType ToolType);
	void ReloadEnd();
	
	UFUNCTION(BlueprintImplementableEvent)
	void ToggleScopeOverlay(bool bShow);
	
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
	virtual void ToggleSpellIndicator_Implementation(bool bShow, UMaterialInterface* DecalMaterial, float Radius = 125.f,  float Range = 0.f) override;
	virtual void PickupAmmunition_Implementation(EToolType ToolType, int32 AmmunitionAmount) override;
	virtual void UpdateInventorySlot_Implementation(EContainerType ContainerType, int32 SlotIndex, FInventoryItemData ItemData) override;
	virtual void OnSlotDrop_Implementation(EContainerType TargetContainer, EContainerType SourceContainer, int32 SourceSlotIndex, int32 TargetSlotIndex, EArmorType ArmorType) override;
	virtual void ResetInventorySlot_Implementation(EContainerType ContainerType, int32 SlotIndex) override;
	void PlayMontage_Implementation(UAnimMontage* Montage) override;
	virtual FTaggedMontage GetTaggedMontage_Implementation() override;
	//~ Player Interface

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastHitReact();
	
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPlayMontage(UAnimMontage* Montage);
	
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

	/*
	 * Inventory
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="PlayerCharacter|Inventory")
	TObjectPtr<UPlayerInventoryComponent> PlayerInventory;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="PlayerCharacter|Hotbar")
	TObjectPtr<UHotbarComponent> HotbarComponent;

	UFUNCTION(Server, Reliable)
	void ServerOnSlotDrop(EContainerType TargetContainer, EContainerType SourceContainer, int32 SourceSlotIndex, int32 TargetSlotIndex, EArmorType ArmorType);

	void OnSlotDrop(EContainerType TargetContainer, EContainerType SourceContainer, int32 SourceSlotIndex, int32 TargetSlotIndex, EArmorType ArmorType) const;

	UFUNCTION(Server, Reliable)
	void ServerUseHotbarSlot(int32 Index);

	UFUNCTION(Server, Reliable)
	void ServerSpawnEquipment(TSubclassOf<AActor> Class, int32 Index);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastAttachActorToMainHand(AActor* TargetActor, FName MainHandSocket, EAnimationState AnimationState);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastUnequipItem();

	UPROPERTY()
	TObjectPtr<AActor> EquippedTool;
	
	int32 CurrentHotbarIndex = -1;
	bool bIsUsingItem = false;

	bool bFinishedSwapping = false;
	
	UPROPERTY(EditDefaultsOnly, Category="Equipment")
	TMap<EAnimationState, FTaggedMontage> AnimationStateMontages;

	UPROPERTY(EditDefaultsOnly, Category="Equipment")
	FName PreferredCombatSocket = FName("LeftHandSocket");
	
protected:
	virtual void BeginPlay() override;
	void AimOffset(float DeltaTime);
	void SimProxiesTurn();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUp() const;

	virtual void OnRep_IsStunned() override;
	virtual void OnRep_IsBurning() override;
	
	UPROPERTY(Replicated, BlueprintReadWrite, Category="PlayerCharacter")
	bool bIsCasting = false;
	
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
	
	/*
	 *
	 */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> AttachedThrowable;

	/* Camera */
	void HideCharacterIfCameraClose();

	UPROPERTY(EditAnywhere, Category="PlayerCharacter|Camera")
	float CameraThreshold = 200.f;

public:
	ATool* GetEquippedTool();
	FVector GetHitTarget() const;
	bool IsEquipped();
	bool IsAiming();
	bool IsCasting();
	bool IsStunned();
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return SpringArm; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; }
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; }
	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return TurningInPlace; }
	FORCEINLINE bool ShouldRotateRootBone() const { return bRotateRootBone; }
	ECombatState GetCombatState() const;
	void SetCombatState(const ECombatState NewState);
	void SetAnimationState(const EAnimationState NewState);
	FORCEINLINE UStaticMeshComponent* GetAttachedThrowable() const { return AttachedThrowable; }
};
