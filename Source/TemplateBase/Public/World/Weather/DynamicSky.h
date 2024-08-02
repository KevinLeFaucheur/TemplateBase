// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "WeatherData.h"
#include "GameFramework/Actor.h"
#include "DynamicSky.generated.h"

class UVolumetricCloudComponent;
enum class ECloudMode : uint8;
class UPostProcessComponent;

UCLASS()
class TEMPLATEBASE_API ADynamicSky : public AActor
{
	GENERATED_BODY()
	
public:	
	ADynamicSky();
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;

	void HandleDynamics();

	UFUNCTION(BlueprintCallable)
	void HandleSunMoonRotation();

	UFUNCTION(BlueprintCallable)
	void HandleSunMoonVisibility();

	UFUNCTION(BlueprintCallable)
	void HandleNightSettings();
	
	UFUNCTION(BlueprintCallable)
	void HandleCloudsSettings() const;
	
	UFUNCTION(BlueprintCallable)
	void SetCloudsSettings() const;

	UFUNCTION(BlueprintCallable)
	bool IsDayTime() const;

	UFUNCTION(BlueprintCallable)
	bool IsNightTime() const;

	UFUNCTION(BlueprintCallable)
	bool InitializeSkySphereMaterial();

	float DynamicTimeOfDay = 0.f;

	UPROPERTY(EditAnywhere, Category="01-Basic Settings")
	float DayNightCycleSpeed = 10.f;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UDirectionalLightComponent> SunDirectionalLight;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UDirectionalLightComponent> MoonDirectionalLight;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USkyAtmosphereComponent> SkyAtmosphere;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USkyLightComponent> SkyLight;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UExponentialHeightFogComponent> ExponentialHeightFog;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UPostProcessComponent> PostProcessVolume;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> SkySphere;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterialInterface> SkySphereMaterial;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynamicSkySphereMaterial;
	
	UPROPERTY(EditAnywhere, Category="01-Basic Settings", meta=(ClampMin="0.0", ClampMax="24.0", UIMin ="0.0", UIMax="24.0"))
	float TimeOfDay = 9.f;
	
	UPROPERTY(EditAnywhere, Category="01-Basic Settings", meta=(ClampMin="4.0", ClampMax="11.0", UIMin ="4.0", UIMax="11.0"))
	float DawnTime = 6.f;
	
	UPROPERTY(EditAnywhere, Category="01-Basic Settings", meta=(ClampMin="16.0", ClampMax="23.0", UIMin ="16.0", UIMax="23.0"))
	float DuskTime = 18.f;

	UPROPERTY(EditAnywhere, Category="01-Basic Settings", meta=(ClampMin="0.0", ClampMax="360.0", UIMin ="0.0", UIMax="360.0"))
	float SunYaw = 0.f;
	
	UPROPERTY(EditAnywhere, Category="01-Basic Settings", meta=(ClampMin="0.0", ClampMax="360.0", UIMin ="0.0", UIMax="360.0"))
	float MoonYaw = 0.f;
	
	UPROPERTY(EditAnywhere, Category="01-Basic Settings")
	FLinearColor DayTimeRayleighScattering;
	
	UPROPERTY(EditAnywhere, Category="02-Night Settings")
	bool bIsNightSkyVisible = true;
	
	UPROPERTY(EditAnywhere, Category="02-Night Settings")
	bool bIsMoonVisible = true;
	
	UPROPERTY(EditAnywhere, Category="02-Night Settings")
	FLinearColor MoonTint = FLinearColor::White;

	UPROPERTY(EditAnywhere, Category="02-Night Settings")
	float MoonScale = .15f;
	
	UPROPERTY(EditAnywhere, Category="02-Night Settings")
	float MoonOrientation = 1.f;
	
	UPROPERTY(EditAnywhere, Category="02-Night Settings")
	float MoonBrightness = 2.f;
	
	UPROPERTY(EditAnywhere, Category="02-Night Settings")
	float MoonLightIntensity = 2.f;
	
	UPROPERTY(EditAnywhere, Category="02-Night Settings")
	float MoonLightSourceAngle = 0.f;
	
	UPROPERTY(EditAnywhere, Category="02-Night Settings")
	float MoonLightTemperature = 8900.f;
	
	UPROPERTY(EditAnywhere, Category="02-Night Settings")
	FLinearColor MoonLightColor;
	
	UPROPERTY(EditAnywhere, Category="02-Night Settings")
	FLinearColor NightTimeRayleighScattering;
	
	UPROPERTY(EditAnywhere, Category="02-Night Settings")
	float NightTimeMultiScatteringFactor = 0.5f;

	/*
	 * Clouds
	 */
	UPROPERTY(EditAnywhere)
	TObjectPtr<UVolumetricCloudComponent> VolumetricCloudComponent;
	
	UPROPERTY(EditAnywhere, Category="03-Clouds Settings")
	ECloudMode CurrentCloudMode = ECloudMode::Clouds2D;
	
	UPROPERTY(EditAnywhere, Category="03-Clouds Settings")
	float Clouds2D_PanningSpeed = 1.f;
	
	UPROPERTY(EditAnywhere, Category="03-Clouds Settings")
	float Clouds2D_Brightness = 1.f;
	
	UPROPERTY(EditAnywhere, Category="03-Clouds Settings", meta=(ClampMin="0.0", ClampMax="1.0", UIMin ="0.0", UIMax="1.0"))
	float Clouds2D_DayTimeCloudsTintStrength = 0.1f;
	
	UPROPERTY(EditAnywhere, Category="03-Clouds Settings", meta=(ClampMin="0.0", ClampMax="1.0", UIMin ="0.0", UIMax="1.0"))
	float Clouds2D_NightTimeCloudsTintStrength = 0.95f;
public:	

};
