// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TransientHarvestable.generated.h"

UCLASS()
class TEMPLATEBASE_API ATransientHarvestable : public AActor
{
	GENERATED_BODY()
	
public:	
	ATransientHarvestable();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(Replicated)
	FVector Direction = FVector::ZeroVector;

protected:
	virtual void BeginPlay() override;
};
