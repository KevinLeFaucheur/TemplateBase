#pragma once

#include "CoreMinimal.h"
#include "WeatherData.generated.h"

UENUM()
enum class ECloudMode : uint8
{
	None UMETA(DisplayName="None"),
	Clouds2D UMETA(DisplayName="2D Clouds"),
	VolumetricClouds UMETA(DisplayName="VolumetricClouds"),
};
