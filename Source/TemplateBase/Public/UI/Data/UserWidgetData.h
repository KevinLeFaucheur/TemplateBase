#pragma once

#include "CoreMinimal.h"
#include "UserWidgetData.generated.h"

UENUM(BlueprintType)
enum class ENotificationType : uint8
{
	Loot UMETA(DisplayName="Loot"),
	AttributePoint UMETA(DisplayName="AttributePoint"),
	SpellPoint UMETA(DisplayName="SpellPoint"),
	LevelUp UMETA(DisplayName="LevelUp"),
	Experience UMETA(DisplayName="Experience"),
};
