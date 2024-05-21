// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipmentComponent.generated.h"

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

private:
	TObjectPtr<APlayerCharacter> PlayerCharacter;

	UPROPERTY(Replicated)
	TObjectPtr<ATool> EquippedTool;

public:
};
