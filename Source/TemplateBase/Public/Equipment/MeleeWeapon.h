// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "Equipment/Weapon.h"
#include "Interaction/CombatInterface.h"
#include "MeleeWeapon.generated.h"

class UBoxComponent;
/**
 * 
 */
UCLASS()
class TEMPLATEBASE_API AMeleeWeapon : public AWeapon
{
	GENERATED_BODY()

public:
	AMeleeWeapon();
	
	UFUNCTION()
	virtual  void OnBoxBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult & SweepResult
	);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon|Melee")
	TObjectPtr<UBoxComponent> CollisionBox;

	UPROPERTY(EditDefaultsOnly, Category="Weapon|Melee")
	TArray<FTaggedMontage> TaggedMontages;

	FTaggedMontage TaggedMontage;

protected:
	virtual void BeginPlay() override;
	
private:

	UPROPERTY(VisibleAnywhere, Category="Weapon|Melee")
	TObjectPtr<USceneComponent> BoxTraceStart;
	
	UPROPERTY(VisibleAnywhere, Category="Weapon|Melee")
	TObjectPtr<USceneComponent> BoxTraceEnd;

	UPROPERTY(VisibleAnywhere, Category="Weapon|Melee")
	bool bIsMultiHit = false;

	UPROPERTY()
	TArray<AActor*> IgnoreActors;

public:
	FORCEINLINE UBoxComponent* GetCollisionBox() const { return CollisionBox; }
	virtual UBoxComponent* GetCollisionBox() override { return CollisionBox; }
	virtual void EmptyIgnoreActors() override;
};
