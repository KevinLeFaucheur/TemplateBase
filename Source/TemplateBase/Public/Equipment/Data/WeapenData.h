#pragma once

#include "CoreMinimal.h"

#define TRACE_LENGTH 80000.f

UENUM(BlueprintType)
enum class EToolType : uint8
{
	ETT_Handgun UMETA(DisplayName="Handgun"),
	ETT_SubMachineGun UMETA(DisplayName="SubMachineGun"),
	ETT_AssaultRifle UMETA(DisplayName="AssaultRifle"),
	ETT_BoltAction UMETA(DisplayName="BoltAction"),
	ETT_PumpAction UMETA(DisplayName="PumpAction"),
	ETT_RocketLauncher UMETA(DisplayName="RocketLauncher"),
	ETT_GrenadeLauncher UMETA(DisplayName="GrenadeLauncher"),
	
	ETT_MAX UMETA(DisplayName="DefaultMAX")
};