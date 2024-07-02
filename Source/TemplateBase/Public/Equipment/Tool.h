// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "EquipmentData.h"
#include "Data/ToolInfo.h"
#include "Data/WeapenData.h"
#include "GameFramework/Actor.h"
#include "Interaction/EquipmentInterface.h"
#include "World/ItemBase.h"
#include "Tool.generated.h"

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
	 * Cosmetics
	 */
	UPROPERTY(EditAnywhere, Category="01-Equipment|Animation")
	TObjectPtr<UAnimationAsset> ActiveAnimation;

	UPROPERTY(EditAnywhere, Category="01-Equipment|SFX")
	TObjectPtr<USoundBase> EquipSound;

	UPROPERTY(EditAnywhere, Category="01-Equipment|SFX")
	TObjectPtr<USoundBase> DropSound;

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
	
	UPROPERTY()
	APlayerCharacterController* OwnerController;
	
	UPROPERTY()
	APlayerCharacter* OwnerCharacter;

	UPROPERTY(VisibleAnywhere, Category="01-Equipment")
	TObjectPtr<USkeletalMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, Category="01-Equipment")
	TObjectPtr<USphereComponent> AreaSphere;

	UPROPERTY(VisibleAnywhere, Category="01-Equipment")
	TObjectPtr<UWidgetComponent> PickupWidget;

private:
	UPROPERTY(EditDefaultsOnly, Category="01-Equipment")
	EToolType ToolType = EToolType::ETT_Handgun;
	
	UPROPERTY(EditDefaultsOnly, Category="01-Equipment")
	EToolClass ToolClass = EToolClass::Tool;
	
	UPROPERTY(ReplicatedUsing=OnRep_ToolState, VisibleAnywhere, Category="01-Equipment")
	EToolState ToolState = EToolState::ETS_Initial;

	UFUNCTION()
	void OnRep_ToolState();
	
	UPROPERTY(EditDefaultsOnly, Category="01-Equipment")
	bool bUsePhysicsAsset = false;
	
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
	FORCEINLINE EToolClass GetToolClass() const { return ToolClass; }
	FORCEINLINE EAnimationState GetAnimationState() const { return AnimationState; }
	FORCEINLINE FName GetMainHandSocket() const { return MainHandSocket; }
	FORCEINLINE FName GetOffhandSocket() const { return OffhandSocket; }

	/*
	 * Meant to be overriden
	*/
	virtual void AddAmmunition(int32 AmountToAdd) {}
	virtual void SetHUDAmmunition() {}
	virtual int32 GetAmmunition() const { return 0; }
	virtual int32 GetAmmunitionCapacity() const { return 0; }
	virtual FName GetReloadEndSection() const { return FName(); }
	virtual bool IsEmpty() const { return false; }
	virtual bool IsFull() const { return false; }
	virtual bool IsAutomatic() const { return false; }
	virtual bool CanInterruptReload() const { return false; }
	virtual bool HasScope() const { return false; }
	virtual float GetFireInterval() const { return 0.f; }
};
