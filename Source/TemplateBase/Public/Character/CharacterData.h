#pragma once

#include "CoreMinimal.h"
#include "CharacterData.generated.h"

UENUM(BlueprintType)
enum class ETurningInPlace : uint8
{
	ETIP_NotTurning UMETA(DisplayName="Not Turning"),
	ETIP_Left UMETA(DisplayName="Turning Left"),
	ETIP_Right UMETA(DisplayName="Turning Right"),
	
	ETIP_MAX UMETA(DisplayName="DefaultMAX")
};

UENUM(BlueprintType)
enum class ECombatState : uint8
{
	ECS_Unoccupied UMETA(DisplayName="Unoccupied"),
	ECS_Reloading UMETA(DisplayName="Reloading"),
	
	ECS_MAX UMETA(DisplayName="DefaultMAX")
};
