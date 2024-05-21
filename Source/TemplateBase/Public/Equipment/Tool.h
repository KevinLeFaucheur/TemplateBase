// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tool.generated.h"

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

protected:
	virtual void BeginPlay() override;

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
	UPROPERTY(VisibleAnywhere, Category="Equipment")
	EToolState ToolState = EToolState::ETS_Initial;

public:	

};
