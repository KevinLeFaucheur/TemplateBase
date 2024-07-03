// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "Character/CharacterData.h"
#include "Components/ActorComponent.h"
#include "UI/PlayerHUD.h"
#include "EquipmentComponent.generated.h"

class AProjectile;
enum class EToolType : uint8;
class APlayerHUD;
class APlayerCharacterController;
class ATool;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEMPLATEBASE_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEquipmentComponent();
	friend class APlayerCharacter;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void EquipTool(ATool* ToolToEquip);
	void DropEquippedTool();
	void SwapTools();
	
	UFUNCTION(BlueprintCallable)
	void SwappingTools();
	
	void Reload();

	UFUNCTION(BlueprintCallable)
	void ReloadSingle();
	
	UFUNCTION(BlueprintCallable)
	void ReloadEnd();
	
	void JumpToReloadEnd() const;
	
	void UpdateAmmunition();
	void UpdateSingleAmmunition();
	void UpdateCarriedAmmunition();
	void SetHUDCarriedAmmunition();
	
	UFUNCTION(BlueprintCallable)
	void ThrowEnd();

	UFUNCTION(BlueprintCallable)
	void Throwing();

	UFUNCTION(Server, Reliable)
	void ServerThrowing(const FVector_NetQuantize& Target);

	void PickupAmmunition(EToolType ToolType, int32 AmmunitionAmount);

	UFUNCTION(BlueprintCallable)
	void SetMeleeWeaponCollisionEnabled(const ECollisionEnabled::Type CollisionEnabled);

protected:
	virtual void BeginPlay() override;

	/*
	 * Equipping
	 */
	void EquipPrimaryTool(ATool* ToolToEquip);
	void EquipSecondaryTool(ATool* ToolToEquip);

	/*
	 * Activating
	 */
	UFUNCTION()
	void OnRep_EquippedTool();
	
	UFUNCTION()
	void OnRep_SecondaryTool();
	
	void AttachToolToSocket(AActor* Tool, const FName& SocketName);
	
	void ActivateTool();

	UFUNCTION(Server, Reliable)
	void ServerActivate(const FVector_NetQuantize& TraceHitTarget);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastActivate(const FVector_NetQuantize& TraceHitTarget);

	UFUNCTION(Server, Reliable)
	void ServerActivateTool();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastActivateTool();
	
	/*
	 * Reloading
	 */
	UFUNCTION(Server, Reliable)
	void ServerReload();

	void HandleReload();

	int32 AmountToReload();
	
	/*
	 * Aiming
	 */
	void TraceUnderCrosshairs(FHitResult& TraceHitResult);
	void SetHUDCrosshairs(float DeltaTime);
	void SetAiming(bool bIsAiming);

	UFUNCTION(Server, Reliable)
	void ServerSetAiming(bool bIsAiming);

	/*
	 * Throwing
	 */
	void Throw();

	UFUNCTION(Server, Reliable)
	void ServerThrow();
	
	void ToggleAttachedThrowable(bool bShow);

private:
	TObjectPtr<APlayerCharacter> PlayerCharacter;
	UPROPERTY() APlayerCharacterController* PlayerCharacterController;
	UPROPERTY() APlayerHUD* PlayerHUD;

	UPROPERTY(ReplicatedUsing=OnRep_CombatState)
	ECombatState CombatState = ECombatState::ECS_Unoccupied;

	UFUNCTION()
	void OnRep_CombatState();

	UPROPERTY(ReplicatedUsing=OnRep_EquippedTool)
	TObjectPtr<ATool> EquippedTool;
	
	UPROPERTY(ReplicatedUsing=OnRep_SecondaryTool)
	TObjectPtr<ATool> SecondaryTool;

	void FireButtonPressed(bool bPressed);

	bool bFireButtonPressed;

	UPROPERTY(Replicated)
	bool bAiming;

	UPROPERTY(EditAnywhere, Category="PlayerCharacter")
	float BaseWalkSpeed;
	
	UPROPERTY(EditAnywhere, Category="PlayerCharacter")
	float AimWalkSpeed;

	UPROPERTY(EditAnywhere, Category="Equipment|Properties")
	float TraceStartAdjustment = 85.f;
	
	FVector HitTarget;

	/*
	 * Throw
	 */
	UPROPERTY(EditAnywhere, Category="Equipment|Throw")
	TSubclassOf<AProjectile> ThrowableClass;
	
	UPROPERTY(EditAnywhere, Category="Equipment|Throw")
	int32 MaxThrowableCount = 4;

	UPROPERTY(ReplicatedUsing=OnRep_ThrowableCount)
	int32 ThrowableCount = 4;

	UFUNCTION()
	void OnRep_ThrowableCount();

	void UpdateHUDThrowableCount();
	
	/*
	 * Crosshairs
	 */
	UPROPERTY(EditAnywhere, Category="Equipment|Crosshairs")
	TObjectPtr<UTexture2D> CrosshairsCenter;

	FHUDPackage HUDPackage;
	
	float CrosshairVelocityFactor;
	float CrosshairAirborneFactor;
	float CrosshairMarksmanFactor;
	float CrosshairPerShotFactor;

	/*
	 * Aiming and FoV
	 */
	float DefaultFOV;
	float CurrentFOV;
	
	UPROPERTY(EditDefaultsOnly, Category="Equipment|Properties")
	float MarksmanFOV = 30.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Equipment|Properties")
	float MarksmanInterpSpeed = 20.f;

	void InterpFOV(float DeltaTime);

	/*
	 * Automatic Fire
	 */
	FTimerHandle FireIntervalTimer;

	void FireIntervalStart();
	void FireIntervalEnd();

	bool bCanActivate = true;
	bool CanActivate();

	UPROPERTY(ReplicatedUsing=OnRep_CarriedAmmunition)
	int32 CarriedAmmunition;

	UFUNCTION()
	void OnRep_CarriedAmmunition();
	
	UPROPERTY(EditDefaultsOnly, Category="Equipment|Ammunition")
	int32 MaxCarriedAmmunition = 100;

	UPROPERTY(EditDefaultsOnly, Category="Equipment|Ammunition")
	int32 StartingHandgunAmmunition = 12;
	
	UPROPERTY(EditDefaultsOnly, Category="Equipment|Ammunition")
	int32 StartingSubMachineGunAmmunition = 30;
	
	UPROPERTY(EditDefaultsOnly, Category="Equipment|Ammunition")
	int32 StartingAssaultRifleAmmunition = 30;
	
	UPROPERTY(EditDefaultsOnly, Category="Equipment|Ammunition")
	int32 StartingGrenadeLauncherAmmunition = 8;
	
	UPROPERTY(EditDefaultsOnly, Category="Equipment|Ammunition")
	int32 StartingRocketLauncherAmmunition = 4;
	
	UPROPERTY(EditDefaultsOnly, Category="Equipment|Ammunition")
	int32 StartingShotgunAmmunition = 15;
	
	UPROPERTY(EditDefaultsOnly, Category="Equipment|Ammunition")
	int32 StartingHighCaliberAmmunition = 8;

	UPROPERTY(EditDefaultsOnly, Category="Equipment|Ammunition")
	TMap<EToolType, int32> CarriedAmmunitionMap;

	void InitializeCarriedAmmunition();
	
public:
	bool ShouldSwapTools();
};
