// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tool.generated.h"

class ACasing;
class UWidgetComponent;

UENUM(BlueprintType)
enum class EToolState : uint8
{
	ETS_Initial UMETA(DisplayName="Initial"),
	ETS_Equipped UMETA(DisplayName="Equipped"),	
	ETS_Dropped UMETA(DisplayName="Dropped"),
	
	ETS_MAX UMETA(DisplayName="DefaultMAX")
};

class USphereComponent;

UCLASS()
class TEMPLATEBASE_API ATool : public AActor
{
	GENERATED_BODY()
	
public:	
	ATool();
	virtual void Tick(float DeltaTime) override;
	void ShowPickupWidget(bool bShowWidget);
	virtual void Activate(const FVector& HitTarget);

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
	 * Automatic
	 */
	UPROPERTY(EditAnywhere, Category="Equipment|Properties")
	float FireInterval = .35f;

	UPROPERTY(EditAnywhere, Category="Equipment|Properties")
	bool bAutomatic = false;
	
	/*
	 * Cosmetics
	 */
	UPROPERTY(EditAnywhere, Category="Equipment|Animation")
	TObjectPtr<UAnimationAsset> ActiveAnimation;

	UPROPERTY(EditAnywhere, Category="Equipment")
	TSubclassOf<ACasing> CasingClass;

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

	UPROPERTY(VisibleAnywhere, Category="Equipment")
	TObjectPtr<USkeletalMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, Category="Equipment")
	TObjectPtr<USphereComponent> AreaSphere;

	UPROPERTY(VisibleAnywhere, Category="Equipment")
	TObjectPtr<UWidgetComponent> PickupWidget;

private:
	UPROPERTY(ReplicatedUsing=OnRep_ToolState, VisibleAnywhere, Category="Equipment")
	EToolState ToolState = EToolState::ETS_Initial;

	UFUNCTION()
	void OnRep_ToolState();

public:	
	void SetToolState(const EToolState NewState);
	FORCEINLINE USphereComponent* GetAreaSphere() { return AreaSphere; }
	FORCEINLINE USkeletalMeshComponent* GetMesh() const { return Mesh; }
	FORCEINLINE float GetMarksmanFOV() const { return MarksmanFOV; }
	FORCEINLINE float GetMarksmanInterpSpeed() const { return MarksmanInterpSpeed; }
};
