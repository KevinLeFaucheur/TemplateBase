// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "WeapenData.h"
#include "AbilitySystem/AbilityTypes.h"
#include "Data/ItemInfo.h"
#include "Data/ToolInfo.h"
#include "Engine/DataAsset.h"
#include "WeaponInfo.generated.h"

class UGameplayEffect;
class ACasing;

USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_BODY()

public:	
	/*
	 * Damage
	*/
	UPROPERTY(EditDefaultsOnly, Category="Properties")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category="Properties", meta=(Categories="Damage"))
	FGameplayTag DamageType;
	
	UPROPERTY(EditDefaultsOnly, Category="Properties")
	FDamageRange DamageRange;
	
	UPROPERTY(EditDefaultsOnly, Category="Properties")
	EToolType ToolType = EToolType::ETT_Handgun;
	
	/*
	 * Zoomed FoV while Aiming
	 */
	UPROPERTY(EditDefaultsOnly, Category="Properties")
	float MarksmanFOV = 30.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Properties")
	float MarksmanInterpSpeed = 20.f;

	/*
	 * Automatic
	 */
	UPROPERTY(EditDefaultsOnly, Category="Properties")
	float FireInterval = .35f;

	UPROPERTY(EditDefaultsOnly, Category="Properties")
	bool bAutomatic = false;

	UPROPERTY(EditDefaultsOnly, Category="Properties")
	int32 Ammunition = 0;

	UPROPERTY(EditDefaultsOnly, Category="Properties")
	int32 AmmunitionCapacity = 0;

	/*
	 * Scatter
	*/
	UPROPERTY(EditAnywhere, Category="Scatter")
	bool bUseScatter = false;
	
	UPROPERTY(EditAnywhere, Category="Scatter")
	bool bDebugScatter = false;
	
	UPROPERTY(EditAnywhere, Category="Scatter")
	float DistanceToSphere = 1000.f;
	
	UPROPERTY(EditAnywhere, Category="Scatter")
	float SphereRadius = 75.f;

	/*
	 * Scope
	 */
	UPROPERTY(EditAnywhere, Category="Mods")
	bool bHasScope = false;
	
	/*
	 * Cosmetics
	*/
	UPROPERTY(EditDefaultsOnly, Category="VFX")
	TSubclassOf<ACasing> CasingClass;
	
	UPROPERTY(VisibleAnywhere, Category="VFX")
	TObjectPtr<USkeletalMeshComponent> Mesh;
	
	UPROPERTY(EditDefaultsOnly, Category="VFX")
	TObjectPtr<UAnimationAsset> ActiveAnimation;

	UPROPERTY(EditDefaultsOnly, Category="SFX")
	TObjectPtr<USoundBase> EquipSound;

	UPROPERTY(EditDefaultsOnly, Category="SFX")
	TObjectPtr<USoundBase> DropSound;
	
	UPROPERTY(EditDefaultsOnly, Category="VFX")
	bool bUsePhysicsAsset = false;

	UPROPERTY(EditDefaultsOnly, Category="Reload")
	bool bCanInterruptReload = false;

	UPROPERTY(EditDefaultsOnly, Category="Reload")
	FName ReloadEndSection = FName("ReloadEnd");
	
	/*
	 * Crosshair Textures
	 */
	UPROPERTY(EditDefaultsOnly, Category="Crosshairs")
	TObjectPtr<UTexture2D> Crosshair_Center;
	
	UPROPERTY(EditDefaultsOnly, Category="Crosshairs")
	TObjectPtr<UTexture2D> Crosshair_Left;
	
	UPROPERTY(EditDefaultsOnly, Category="Crosshairs")
	TObjectPtr<UTexture2D> Crosshair_Top;
	
	UPROPERTY(EditDefaultsOnly, Category="Crosshairs")
	TObjectPtr<UTexture2D> Crosshair_Right;
	
	UPROPERTY(EditDefaultsOnly, Category="Crosshairs")
	TObjectPtr<UTexture2D> Crosshair_Bottom;
};

/**
 * 
 */
UCLASS()
class TEMPLATEBASE_API UWeaponInfo : public UToolInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	FWeaponData WeaponData;

	// FWeaponInfo FindWeaponInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound = false) const;
	
};
