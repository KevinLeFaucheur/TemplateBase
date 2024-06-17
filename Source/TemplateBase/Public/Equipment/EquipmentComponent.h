// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UI/PlayerHUD.h"
#include "EquipmentComponent.generated.h"

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

protected:
	virtual void BeginPlay() override;
	void SetAiming(bool bIsAiming);

	UFUNCTION(Server, Reliable)
	void ServerSetAiming(bool bIsAiming);

	UFUNCTION()
	void OnRep_EquippedTool();
	void ActivateTool();

	UFUNCTION(Server, Reliable)
	void ServerActivate(const FVector_NetQuantize& TraceHitTarget);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastActivate(const FVector_NetQuantize& TraceHitTarget);

	void TraceUnderCrosshairs(FHitResult& TraceHitResult);

	void SetHUDCrosshairs(float DeltaTime);

private:
	TObjectPtr<APlayerCharacter> PlayerCharacter;
	UPROPERTY() APlayerCharacterController* PlayerCharacterController;
	UPROPERTY() APlayerHUD* PlayerHUD;

	UPROPERTY(ReplicatedUsing=OnRep_EquippedTool)
	TObjectPtr<ATool> EquippedTool;

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
	
	UPROPERTY(EditAnywhere, Category="Equipment|Properties")
	float MarksmanFOV = 30.f;
	
	UPROPERTY(EditAnywhere, Category="Equipment|Properties")
	float MarksmanInterpSpeed = 20.f;

	void InterpFOV(float DeltaTime);

	/*
	 * Automatic Fire
	 */
	FTimerHandle FireIntervalTimer;

	void FireIntervalStart();
	void FireIntervalEnd();

	bool bCanFire = true;
	
public:
};
