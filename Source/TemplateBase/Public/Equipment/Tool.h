// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "EquipmentData.h"
#include "GameplayTagContainer.h"
#include "Data/ToolInfo.h"
#include "Data/WeapenData.h"
#include "GameFramework/Actor.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/EquipmentInterface.h"
#include "World/ItemBase.h"
#include "Tool.generated.h"

class UGameplayAbility;
class UBoxComponent;
enum class EToolClass : uint8;
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
	void InitializeTool();
	ATool();
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Tick(float DeltaTime) override;
	void PlayFireAnimation();
	void ShowPickupWidget(bool bShowWidget);
	virtual void Activate();
	virtual void Activate(const FVector& HitTarget);
	virtual void Drop();
	virtual void OnRep_Owner() override;

	//~ Equipment Interface
	virtual FEquipmentInfo GetEquipmentInfo_Implementation() override;
	virtual void UseItem_Implementation(AActor* PlayerCharacter) override;
	void MontageEnd_Implementation() override;
	//~ Equipment Interface

	UPROPERTY(EditDefaultsOnly, Category="Equipment")
	bool bUseAimOffsets = false;

	// TODO: Should be DataAsset/Table
	/*
	 * Crosshairs
	 */
	UPROPERTY(EditAnywhere, Category="Equipment|Crosshairs")
	TObjectPtr<UTexture2D> CrosshairsCenter;
	
	UPROPERTY(EditAnywhere, Category="Equipment|Crosshairs")
	TObjectPtr<UTexture2D> CrosshairsLeft;
	
	UPROPERTY(EditAnywhere, Category="Equipment|Crosshairs")
	TObjectPtr<UTexture2D> CrosshairsRight;
	
	UPROPERTY(EditAnywhere, Category="Equipment|Crosshairs")
	TObjectPtr<UTexture2D> CrosshairsTop;
	
	UPROPERTY(EditAnywhere, Category="Equipment|Crosshairs")
	TObjectPtr<UTexture2D> CrosshairsBottom;

	/*
	 * Zoomed FoV while Aiming
	 */
	UPROPERTY(EditAnywhere, Category="Equipment|Properties")
	float MarksmanFOV = 30.f;
	
	UPROPERTY(EditAnywhere, Category="Equipment|Properties")
	float MarksmanInterpSpeed = 20.f;
	
	/*
	 * Abilities
	 */
	UPROPERTY(EditDefaultsOnly, Category="Equipment|Abilities")
	TMap<FGameplayTag, FGameplayTag> ToolAbilities;
	
	/*
	 * Cosmetics
	 */
	UPROPERTY(EditAnywhere, Category="Equipment|Animation")
	TObjectPtr<UAnimationAsset> ActiveAnimation;

	UPROPERTY(EditAnywhere, Category="Equipment|SFX")
	TObjectPtr<USoundBase> EquipSound;

	UPROPERTY(EditAnywhere, Category="Equipment|SFX")
	TObjectPtr<USoundBase> DropSound;
	
	UPROPERTY(EditDefaultsOnly, Category="Equipment|Physics")
	bool bUsePhysicsAsset = false;

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

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

	virtual void OnToolStateSet();
	
	UPROPERTY()
	APlayerCharacterController* OwnerController = nullptr;
	
	UPROPERTY()
	APlayerCharacter* OwnerCharacter = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category="Equipment")
	EToolClass ToolClass = EToolClass::Tool;

	UPROPERTY(VisibleAnywhere, Category="Equipment")
	TObjectPtr<USphereComponent> AreaSphere;

	UPROPERTY(VisibleAnywhere, Category="Equipment")
	TObjectPtr<UWidgetComponent> PickupWidget;

private:
	UFUNCTION()
	void OnRep_ToolState();
	
	UPROPERTY(EditDefaultsOnly, Category="Equipment")
	EAnimationState AnimationState = EAnimationState::Default;
	
	UPROPERTY(EditDefaultsOnly, Category="Equipment")
	EToolType ToolType = EToolType::ETT_Handgun;
	
	UPROPERTY(ReplicatedUsing=OnRep_ToolState, VisibleAnywhere, Category="Equipment")
	EToolState ToolState = EToolState::ETS_Initial;

	
	/*
	 * Sockets
	 */
	UPROPERTY(EditDefaultsOnly, Category="Equipment")
	FName MainHandSocket = FName("RightHandSocket");
	
	UPROPERTY(EditDefaultsOnly, Category="Equipment|Reload")
	FName OffhandSocket = FName("OffhandSocket");
	
	UPROPERTY(EditDefaultsOnly, Category="Equipment")
	FName BackSocket = FName("BackSocket_01");
	
	UPROPERTY(EditDefaultsOnly, Category="Equipment")
	FName BeltSocket = FName("BeltSocket_01");
	
public:	
	void SetToolState(const EToolState NewState);
	FORCEINLINE USphereComponent* GetAreaSphere() { return AreaSphere; }
	FORCEINLINE float GetMarksmanFOV() const { return MarksmanFOV; }
	FORCEINLINE float GetMarksmanInterpSpeed() const { return MarksmanInterpSpeed; }
	FORCEINLINE EToolType GetToolType() const { return ToolType; }
	FORCEINLINE EToolClass GetToolClass() const { return ToolClass; }
	FORCEINLINE EAnimationState GetAnimationState() const { return AnimationState; }
	FORCEINLINE FName GetMainHandSocket() const { return MainHandSocket; }
	FORCEINLINE FName GetOffhandSocket() const { return OffhandSocket; }
	FORCEINLINE FName GetBackSocket() const { return BackSocket; }
	FORCEINLINE FName GetBeltSocket() const { return BeltSocket; }
	FORCEINLINE bool HasAbilities() const { return ToolAbilities.Num() > 0; }
	TArray<FGameplayTag> GetToolAbilityTags() const;

	/*
	 * Overriden in RangeWeapon
	*/
	virtual void AddAmmunition(int32 AmountToAdd) {}
	virtual void SetHUDAmmunition() {}
	virtual int32 GetAmmunition() const { return 0; }
	virtual int32 GetAmmunitionCapacity() const { return 0; }
	virtual FName GetReloadEndSection() const { return FName(); }
	virtual bool IsEmpty() const { return false; }
	virtual bool IsFull() const { return false; }
	virtual int32 AmountToFill() const { return 0; }
	virtual bool IsAutomatic() const { return false; }
	virtual bool CanInterruptReload() const { return false; }
	virtual bool HasScope() const { return false; }
	virtual float GetFireInterval() const { return 0.f; }

	/*
	 * Overriden in MeleeWeapon
	 */
	virtual UBoxComponent* GetCollisionBox() { return nullptr; }
	virtual void EmptyIgnoreActors() {};
	
	/*
	 * Overriden in Weapon and HarvestingTool
	 */
	virtual USkeletalMeshComponent* GetMesh() const { return nullptr; }
	virtual void PlayActiveAnimation() {}
	virtual void DetachToolFromComponent() {}
	virtual void OnEquipped();
	virtual void OnDropped();
	virtual void OnSecondary();
};
