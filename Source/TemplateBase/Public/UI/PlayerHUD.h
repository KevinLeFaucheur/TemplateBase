// Retropsis @ 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

USTRUCT(BlueprintType)
struct FHUDPackage
{
	GENERATED_BODY()

public:
	UPROPERTY() TObjectPtr<UTexture2D> CrosshairsCenter;
	UPROPERTY() TObjectPtr<UTexture2D> CrosshairsLeft;
	UPROPERTY() TObjectPtr<UTexture2D> CrosshairsRight;
	UPROPERTY() TObjectPtr<UTexture2D> CrosshairsTop;
	UPROPERTY() TObjectPtr<UTexture2D> CrosshairsBottom;
	float CrosshairSpread;
	FLinearColor Color;
};

/**
 * 
 */
UCLASS()
class TEMPLATEBASE_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

private:
	void DrawCrosshairs(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread, FLinearColor Color);
	
	FHUDPackage HUDPackage;

	UPROPERTY(EditAnywhere, Category="Equipment|Crosshairs")
	float CrosshairSpreadMax = 16.f;

public:
	FORCEINLINE void SetHUDPackage(const FHUDPackage& Package) { HUDPackage = Package; }
};
