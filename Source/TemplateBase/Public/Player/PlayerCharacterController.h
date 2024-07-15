// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "Interaction/ControllerInterface.h"
#include "PlayerCharacterController.generated.h"

class UHealingTextComponent;
class AMagicCircle;
class UDamageTextComponent;
class UBaseAbilitySystemComponent;
class APlayerCharacter;
class UInputAction;
class UInputConfig;
class UInputMappingContext;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAmmunitionChangedSignature, int32 /* NewCount */);

/**
 * 
 */
UCLASS()
class TEMPLATEBASE_API APlayerCharacterController : public APlayerController, public IControllerInterface
{
	GENERATED_BODY()

public:
	virtual void PlayerTick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<UInputAction> LookAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<UInputAction> EquipAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<UInputAction> CrouchAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<UInputAction> FireAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<UInputAction> ReloadAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<UInputAction> ThrowAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<UInputAction> AimAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<UInputAction> ToggleMouseCursorAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<UInputAction> InventoryAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<UInputAction> HotbarAction_0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<UInputAction> HotbarAction_1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<UInputAction> HotbarAction_2;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<UInputAction> HotbarAction_3;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<UInputAction> HotbarAction_4;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<UInputAction> HotbarAction_5;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<UInputAction> HotbarAction_6;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<UInputAction> HotbarAction_7;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<UInputAction> HotbarAction_8;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PlayerCharacter")
	TObjectPtr<UInputAction> HotbarAction_9;
	
	UFUNCTION(Client, Reliable)
	void ClientShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit, float Delay = 0.f);

	UFUNCTION()
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit) const;
	
	UFUNCTION(Client, Reliable)
	void ClientShowHealingNumber(float HealingAmount, ACharacter* TargetCharacter, bool bCriticalHit = false, float Delay = 0.f);

	UFUNCTION()
	void ShowHealingNumber(float HealingAmount, ACharacter* TargetCharacter, bool bCriticalHit = false) const;
	
	void SetHUDAmmunition(int32 Ammo) const;
	void SetHUDCarriedAmmunition(int32 Ammo) const;
	void SetHUDThrowableCount(int32 GrenadeCount) const;
	FOnAmmunitionChangedSignature AmmunitionChanged;
	FOnAmmunitionChangedSignature CarriedAmmunitionChanged;
	FOnAmmunitionChangedSignature ThrowableCountChanged;

	UFUNCTION(BlueprintCallable)
	void ToggleMagicCircle(bool bShow, UMaterialInterface* DecalMaterial = nullptr);

	UPROPERTY(BlueprintReadWrite)
	bool bIsInventoryOpen = false;
	
	virtual void UpdateInventorySlot_Implementation(EContainerType ContainerType, int32 SlotIndex, FInventoryItemData ItemData) override;
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	void CursorTrace();
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump();
	void StopJumping();
	void CrouchButtonPressed();
	void EquipButtonPressed();
	void FireButtonPressed();
	void FireButtonReleased();
	void AimButtonPressed();
	void AimButtonReleased();
	void ReloadButtonPressed();
	void ThrowButtonPressed();
	
	void ToggleMouseCursorPressed();

	UFUNCTION(Client, Reliable)
	void InventoryButtonPressed();

	/*
	 * Hotbar
	 */
	void HotbarButtonPressed(int32 Index);

	UFUNCTION(BlueprintImplementableEvent)
	void ToggleInventory();

private:
	UPROPERTY()
	TObjectPtr<APlayerCharacter> PlayerCharacter;

	UPROPERTY()
	TObjectPtr<UBaseAbilitySystemComponent> BaseAbilitySystemComponent;

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UBaseAbilitySystemComponent* GetASC();

	UPROPERTY(EditDefaultsOnly, Category="PlayerCharacter|Input")
	TObjectPtr<UInputConfig> InputConfig;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UHealingTextComponent> HealingTextComponentClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMagicCircle> MagicCircleClass;

	UPROPERTY()
	TObjectPtr<AMagicCircle> MagicCircle;
	
	FHitResult CursorHit;
	void UpdateMagicCircleLocation();

public:
	FORCEINLINE FVector GetCursorHit() const { return CursorHit.ImpactPoint; }
};


