// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipmentComponent.generated.h"

#define TRACE_LENGTH 80000.f

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

	UFUNCTION(Server, Reliable)
	void ServerActivate(const FVector_NetQuantize& HitTarget);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastFire(const FVector_NetQuantize& HitTarget);

	void TraceUnderCrosshairs(FHitResult& HitResult);

private:
	TObjectPtr<APlayerCharacter> PlayerCharacter;

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

	// FVector HitTarget;

public:
};
