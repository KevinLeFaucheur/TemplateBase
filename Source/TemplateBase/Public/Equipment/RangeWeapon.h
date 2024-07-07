// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "Equipment/Weapon.h"
#include "Interaction/RangeWeaponInterface.h"
#include "RangeWeapon.generated.h"

/**
 * 
 */
UCLASS()
class TEMPLATEBASE_API ARangeWeapon : public AWeapon, public IRangeWeaponInterface
{
	GENERATED_BODY()

public:
	ARangeWeapon();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	FVector TraceEndWithScatter(const FVector& TraceStart, const FVector& HitTarget) const;
	virtual void Activate(const FVector& HitTarget) override;
	void EjectCasing();
	
	virtual void AddAmmunition(int32 AmountToAdd) override;
	virtual void SetHUDAmmunition() override;
	virtual int32 GetAmmunition() const override { return Ammunition; }
	virtual int32 GetAmmunitionCapacity() const override { return AmmunitionCapacity; }	
	virtual FName GetReloadEndSection() const override { return ReloadEndSection; }
	virtual bool CanInterruptReload() const override { return bCanInterruptReload; }
	virtual bool IsEmpty() const override { return Ammunition <= 0; }
	virtual bool IsFull() const override { return Ammunition == AmmunitionCapacity; }
	virtual bool IsAutomatic() const override { return bAutomatic; }
	virtual bool HasScope()const override { return bHasScope; }
	virtual float GetFireInterval() const override { return FireInterval; }

	/*
	 * Automatic
	 */
	UPROPERTY(EditAnywhere, Category="01-Equipment|Properties")
	float FireInterval = .35f;

	UPROPERTY(EditAnywhere, Category="01-Equipment|Properties")
	bool bAutomatic = false;

	/*
	 * Cosmetics
	 */
	UPROPERTY(EditAnywhere, Category="Weapon")
	TSubclassOf<ACasing> CasingClass;

protected:
	/*
	 * Ammunition
	 */
	UPROPERTY(ReplicatedUsing=OnRep_Ammunition, EditDefaultsOnly, Category="01-Equipment")
	int32 Ammunition = 0;

	UFUNCTION()
	void OnRep_Ammunition();
	void SpendAmmunition();

	UPROPERTY(EditDefaultsOnly, Category="01-Equipment")
	int32 AmmunitionCapacity = 0;

	/*
	 * Reloading
	 */
	UPROPERTY(EditDefaultsOnly, Category="01-Equipment|Reload")
	FName ReloadEndSection = FName("ReloadEnd");

	UPROPERTY(EditDefaultsOnly, Category="01-Equipment|Reload")
	bool bCanInterruptReload = false;
	
	/*
	 * Scatter
	*/
	UPROPERTY(EditAnywhere, Category="Weapon|Scatter")
	bool bUseScatter = false;
	
	UPROPERTY(EditAnywhere, Category="Weapon|Scatter")
	bool bDebugScatter = false;
	
	UPROPERTY(EditAnywhere, Category="Weapon|Scatter")
	float DistanceToSphere = 1000.f;
	
	UPROPERTY(EditAnywhere, Category="Weapon|Scatter")
	float SphereRadius = 75.f;

	/*
	 * Scope
	 */
	UPROPERTY(EditAnywhere, Category="Weapon|Mods")
	bool bHasScope = false;

public:
	
};
