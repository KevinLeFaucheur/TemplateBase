// Retropsis @ 2024

#include "World/Weather/DynamicSky.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/PostProcessComponent.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/VolumetricCloudComponent.h"
#include "Kismet/KismetMathLibrary.h"

ADynamicSky::ADynamicSky()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");

	SunDirectionalLight = CreateDefaultSubobject<UDirectionalLightComponent>("SunDirectionalLight");
	SunDirectionalLight->ForwardShadingPriority = 1;
	SunDirectionalLight->SetupAttachment(GetRootComponent());
	SunDirectionalLight->SetRelativeRotation(FRotator(-75.f, 0.f, 0.f));
	
	MoonDirectionalLight = CreateDefaultSubobject<UDirectionalLightComponent>("MoonDirectionalLight");
	MoonDirectionalLight->ForwardShadingPriority = 1;
	MoonDirectionalLight->SetupAttachment(GetRootComponent());
	
	SkyAtmosphere = CreateDefaultSubobject<USkyAtmosphereComponent>("SkyAtmosphere");
	SkyAtmosphere->SetupAttachment(GetRootComponent());
	
	SkyLight = CreateDefaultSubobject<USkyLightComponent>("SkyLight");
	SkyLight->SetRealTimeCaptureEnabled(true);
	SkyLight->SetupAttachment(GetRootComponent());

	ExponentialHeightFog = CreateDefaultSubobject<UExponentialHeightFogComponent>("ExponentialHeightFog");
	ExponentialHeightFog->SetupAttachment(GetRootComponent());
	
	PostProcessVolume = CreateDefaultSubobject<UPostProcessComponent>("PostProcessVolume");
	PostProcessVolume->SetupAttachment(GetRootComponent());
	PostProcessVolume->Settings.AutoExposureMinBrightness = .5f;
	PostProcessVolume->Settings.AutoExposureMaxBrightness = .5f;
	PostProcessVolume->Settings.bOverride_AutoExposureMinBrightness = true;
	PostProcessVolume->Settings.bOverride_AutoExposureMaxBrightness = true;

	SkySphere = CreateDefaultSubobject<UStaticMeshComponent>("SkySphere");
	SkySphere->SetupAttachment(GetRootComponent());
	SkySphere->SetWorldScale3D(FVector(100000.f));

	VolumetricCloudComponent = CreateDefaultSubobject<UVolumetricCloudComponent>("VolumetricClouds");
	VolumetricCloudComponent->SetupAttachment(GetRootComponent());
}

void ADynamicSky::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if(InitializeSkySphereMaterial())
	{
		HandleSunMoonRotation();
		HandleSunMoonVisibility();
		HandleNightSettings();
		HandleCloudsSettings();
	}
}

void ADynamicSky::BeginPlay()
{
	Super::BeginPlay();
	InitializeSkySphereMaterial();
	DynamicTimeOfDay = TimeOfDay;
}

void ADynamicSky::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DynamicTimeOfDay += DeltaTime * DayNightCycleSpeed;
	double Remainder;
	const int64 Days = UKismetMathLibrary::FMod64(DynamicTimeOfDay, 24.f, Remainder);
	GEngine->AddOnScreenDebugMessage(12345, 1.f, FColor::White, FString::Printf(TEXT("Current Time: %f - Days: %lld"), Remainder, Days));
	const FText TimeFormat = FText::AsTimespan(UKismetMathLibrary::FromHours(Remainder));
	GEngine->AddOnScreenDebugMessage(12346, 1.f, FColor::White, TimeFormat.ToString());

	TimeOfDay = Remainder;
	HandleDynamics();
}

void ADynamicSky::HandleDynamics()
{
	HandleSunMoonRotation();
	HandleSunMoonVisibility();
	HandleNightSettings();
	HandleCloudsSettings();
}

void ADynamicSky::HandleSunMoonRotation()
{
	const float SunPitch = UKismetMathLibrary::MapRangeUnclamped(TimeOfDay, DawnTime, DuskTime, 0.f, -180.f);
	SunDirectionalLight->SetWorldRotation(FRotator(SunPitch, SunYaw, 0.f));
	
	const float MoonPitchAfterDusk = UKismetMathLibrary::MapRangeUnclamped(TimeOfDay, DuskTime, 24.f, -180.f, -90.f);
	MoonDirectionalLight->SetWorldRotation(FRotator(MoonPitchAfterDusk, MoonYaw, 0.f));
	
	const float MoonPitchBeforeDawn = UKismetMathLibrary::MapRangeUnclamped(TimeOfDay, 0.f, DawnTime, -90.f, 0.f);
	MoonDirectionalLight->SetWorldRotation(FRotator(MoonPitchBeforeDawn, MoonYaw, 0.f));
}

void ADynamicSky::HandleSunMoonVisibility()
{
	SunDirectionalLight->SetVisibility(IsDayTime());
	MoonDirectionalLight->SetVisibility(IsNightTime());
}

void ADynamicSky::HandleNightSettings()
{
	if(IsNightTime())
	{
		MoonDirectionalLight->SetIntensity(1.f);
		MoonDirectionalLight->SetLightColor(MoonLightColor);
		MoonDirectionalLight->SetLightSourceAngle(MoonLightSourceAngle);
		MoonDirectionalLight->SetUseTemperature(true);
		MoonDirectionalLight->Temperature = MoonLightTemperature;

		SkyAtmosphere->SetRayleighScattering(NightTimeRayleighScattering);
		SkyAtmosphere->SetMultiScatteringFactor(NightTimeMultiScatteringFactor);
		
		DynamicSkySphereMaterial->SetScalarParameterValue(FName("IsNightSkyVisible"), bIsNightSkyVisible ? 1.0 : 0.f);
		DynamicSkySphereMaterial->SetScalarParameterValue(FName("IsMoonVisible"), bIsMoonVisible ? 1.0 : 0.f);
		DynamicSkySphereMaterial->SetVectorParameterValue(FName("MoonBasicSettings"), FLinearColor(MoonScale, MoonOrientation, MoonBrightness, 1.f));
		DynamicSkySphereMaterial->SetVectorParameterValue(FName("MoonTint"), MoonTint);
	}
	else
	{
		SkyAtmosphere->SetRayleighScattering(DayTimeRayleighScattering);
		SkyAtmosphere->SetMultiScatteringFactor(1.f);
		DynamicSkySphereMaterial->SetScalarParameterValue(FName("IsNightSkyVisible"), 0.f);
		DynamicSkySphereMaterial->SetScalarParameterValue(FName("IsMoonVisible"), 0.f);
	}
}

bool ADynamicSky::InitializeSkySphereMaterial()
{
	if(SkySphereMaterial)
	{
		DynamicSkySphereMaterial = SkySphere->CreateDynamicMaterialInstance(0, SkySphereMaterial);
	}
	return IsValid(DynamicSkySphereMaterial);
}

bool ADynamicSky::IsDayTime() const
{
	return TimeOfDay > DawnTime && TimeOfDay < DuskTime;
}

bool ADynamicSky::IsNightTime() const
{
	return !IsDayTime();
}

/*
 * Clouds
 */
void ADynamicSky::HandleCloudsSettings() const
{
	switch (CurrentCloudMode) {
	case ECloudMode::None:
	case ECloudMode::VolumetricClouds:
		DynamicSkySphereMaterial->SetScalarParameterValue(FName("AreCloudsVisible"), 0.f);
		break;
	case ECloudMode::Clouds2D:
		DynamicSkySphereMaterial->SetScalarParameterValue(FName("AreCloudsVisible"), 1.f);
		SetCloudsSettings();
		break;
	}
}

void ADynamicSky::SetCloudsSettings() const
{
	const FVector4d CloudsSettings{ 1.f, Clouds2D_PanningSpeed, Clouds2D_Brightness, IsDayTime() ? Clouds2D_DayTimeCloudsTintStrength : Clouds2D_NightTimeCloudsTintStrength };
	DynamicSkySphereMaterial->SetVectorParameterValue(FName("CloudsSettings"), CloudsSettings);
}

