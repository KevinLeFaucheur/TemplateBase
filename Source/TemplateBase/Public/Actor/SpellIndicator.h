// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpellIndicator.generated.h"

UCLASS()
class TEMPLATEBASE_API ASpellIndicator : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpellIndicator();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UDecalComponent> SpellIndicatorDecal;

	void SetDecalSize(float Radius) const;
	
protected:
	virtual void BeginPlay() override;
};
