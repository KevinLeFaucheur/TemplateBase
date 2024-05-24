// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Casing.generated.h"

UCLASS()
class TEMPLATEBASE_API ACasing : public AActor
{
	GENERATED_BODY()
	
public:	
	ACasing();

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnHit(
		UPrimitiveComponent* HitComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit
	);

private:
	UPROPERTY(VisibleAnywhere, Category="Weapon|Projectile")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, Category="Weapon|Projectile")
	float CasingEjectionImpulse = 10.f;

	bool bCasingSoundPlayed = false;
	
	UPROPERTY(EditAnywhere, Category="Weapon|Projectile")
	TObjectPtr<USoundBase> CasingSound;

public:	
};
