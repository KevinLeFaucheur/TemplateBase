// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "EquipmentData.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/AbilityTypes.h"
#include "Data/WeapenData.h"
#include "GameFramework/Actor.h"
#include "Interaction/EquipmentInterface.h"
#include "World/ItemBase.h"
#include "Tool.generated.h"

enum class EToolType : uint8;
class UGameplayEffect;
class APlayerCharacterController;
class APlayerCharacter;
class ACasing;
class UWidgetComponent;
class USphereComponent;

UCLASS()
class TEMPLATEBASE_API ATool : public AItemBase, public IEquipmentInterface
{
	GENERATED_BODY()
	
public:	
	ATool();
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Tick(float DeltaTime) override;
	void PlayFireAnimation();
	void EjectCasing();
	void ShowPickupWidget(bool bShowWidget);
	virtual void Activate(const FVector& HitTarget);
	virtual void Drop();
	virtual void OnRep_Owner() override;
	void AddAmmunition(int32 AmountToAdd);
	void SetHUDAmmunition();

	//~ Equipment Interface
	virtual FEquipmentInfo GetEquipmentInfo_Implementation() override;
	virtual void UseItem_Implementation(AActor* PlayerCharacter) override;
	void MontageEnd_Implementation() override;
	//~ Equipment Interface

	UPROPERTY(EditDefaultsOnly, Category="01-Equipment")
	bool bUseAimOffsets = false;

	// TODO: Should be DataAsset/Table
	/*
	 * Crosshairs
	 */
	UPROPERTY(EditAnywhere, Category="01-Equipment|Crosshairs")
	TObjectPtr<UTexture2D> CrosshairsCenter;
	
	UPROPERTY(EditAnywhere, Category="01-Equipment|Crosshairs")
	TObjectPtr<UTexture2D> CrosshairsLeft;
	
	UPROPERTY(EditAnywhere, Category="01-Equipment|Crosshairs")
	TObjectPtr<UTexture2D> CrosshairsRight;
	
	UPROPERTY(EditAnywhere, Category="01-Equipment|Crosshairs")
	TObjectPtr<UTexture2D> CrosshairsTop;
	
	UPROPERTY(EditAnywhere, Category="01-Equipment|Crosshairs")
	TObjectPtr<UTexture2D> CrosshairsBottom;

	/*
	 * Zoomed FoV while Aiming
	 */
	UPROPERTY(EditAnywhere, Category="01-Equipment|Properties")
	float MarksmanFOV = 30.f;
	
	UPROPERTY(EditAnywhere, Category="01-Equipment|Properties")
	float MarksmanInterpSpeed = 20.f;

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
	UPROPERTY(EditAnywhere, Category="01-Equipment|Animation")
	TObjectPtr<UAnimationAsset> ActiveAnimation;

	UPROPERTY(EditAnywhere, Category="01-Equipment")
	TSubclassOf<ACasing> CasingClass;

	UPROPERTY(EditAnywhere, Category="01-Equipment|SFX")
	TObjectPtr<USoundBase> EquipSound;

	UPROPERTY(EditAnywhere, Category="01-Equipment|SFX")
	TObjectPtr<USoundBase> DropSound;

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FVector TraceEndWithScatter(const FVector& TraceStart, const FVector& HitTarget) const;

	UFUNCTION()
	void OnSphereBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, Category="01-Equipment")
	TObjectPtr<USkeletalMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, Category="01-Equipment")
	TObjectPtr<USphereComponent> AreaSphere;

	UPROPERTY(VisibleAnywhere, Category="01-Equipment")
	TObjectPtr<UWidgetComponent> PickupWidget;

	/*
	 * Scatter
	*/
	UPROPERTY(EditAnywhere, Category="01-Equipment|Scatter")
	bool bUseScatter = false;
	
	UPROPERTY(EditAnywhere, Category="01-Equipment|Scatter")
	bool bDebugScatter = false;
	
	UPROPERTY(EditAnywhere, Category="01-Equipment|Scatter")
	float DistanceToSphere = 1000.f;
	
	UPROPERTY(EditAnywhere, Category="01-Equipment|Scatter")
	float SphereRadius = 75.f;

	/*
	 * Scope
	 */
	UPROPERTY(EditAnywhere, Category="01-Equipment|Mods")
	bool bHasScope = false;

	/*
	 * Damage
	*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="01-Equipment|Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="01-Equipment|Damage", meta=(Categories="Damage"))
	FGameplayTag DamageType;
	
	UPROPERTY(EditDefaultsOnly, Category="01-Equipment|Damage")
	FDamageRange DamageRange;

private:
	UPROPERTY()
	APlayerCharacterController* OwnerController;
	
	UPROPERTY()
	APlayerCharacter* OwnerCharacter;

	UPROPERTY(EditDefaultsOnly, Category="01-Equipment")
	EToolType ToolType = EToolType::ETT_Handgun;
	
	UPROPERTY(ReplicatedUsing=OnRep_ToolState, VisibleAnywhere, Category="01-Equipment")
	EToolState ToolState = EToolState::ETS_Initial;

	UFUNCTION()
	void OnRep_ToolState();

	UPROPERTY(ReplicatedUsing=OnRep_Ammunition, EditDefaultsOnly, Category="01-Equipment")
	int32 Ammunition;

	UFUNCTION()
	void OnRep_Ammunition();

	void SpendAmmunition();

	UPROPERTY(EditDefaultsOnly, Category="01-Equipment")
	int32 AmmunitionCapacity;
	
	UPROPERTY(EditDefaultsOnly, Category="01-Equipment")
	bool bUsePhysicsAsset = false;

	UPROPERTY(EditDefaultsOnly, Category="01-Equipment|Reload")
	FName ReloadEndSection = FName("ReloadEnd");

	UPROPERTY(EditDefaultsOnly, Category="01-Equipment|Reload")
	bool bCanInterruptReload = false;
	
	UPROPERTY(EditDefaultsOnly, Category="01-Equipment")
	FName MainHandSocket = FName("RightHandSocket");
	
	UPROPERTY(EditDefaultsOnly, Category="01-Equipment|Reload")
	FName OffhandSocket = FName("OffhandSocket");

	UPROPERTY(EditDefaultsOnly, Category="01-Equipment")
	EAnimationState AnimationState = EAnimationState::Default;
	
public:	
	void SetToolState(const EToolState NewState);
	FORCEINLINE USphereComponent* GetAreaSphere() { return AreaSphere; }
	FORCEINLINE USkeletalMeshComponent* GetMesh() const { return Mesh; }
	FORCEINLINE float GetMarksmanFOV() const { return MarksmanFOV; }
	FORCEINLINE float GetMarksmanInterpSpeed() const { return MarksmanInterpSpeed; }
	FORCEINLINE EToolType GetToolType() const { return ToolType; }
	FORCEINLINE EAnimationState GetAnimationState() const { return AnimationState; }
	FORCEINLINE bool IsEmpty() const { return Ammunition <= 0; };
	FORCEINLINE bool IsFull() const { return Ammunition == AmmunitionCapacity; };
	FORCEINLINE int32 GetAmmunition() const { return Ammunition; }
	FORCEINLINE int32 GetAmmunitionCapacity() const { return AmmunitionCapacity; }
	FORCEINLINE bool HasScope() const { return bHasScope; }
	FORCEINLINE FName GetReloadEndSection() const { return ReloadEndSection; }
	FORCEINLINE FName GetMainHandSocket() const { return MainHandSocket; }
	FORCEINLINE FName GetOffhandSocket() const { return OffhandSocket; }
	FORCEINLINE bool CanInterruptReload() const { return bCanInterruptReload; }
};
