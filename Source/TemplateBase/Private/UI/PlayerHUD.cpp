// Retropsis @ 2024


#include "UI/PlayerHUD.h"
#include "UI/BaseUserWidget.h"
#include "UI/Controller/AttributeMenuWidgetController.h"
#include "UI/Controller/OverlayWidgetController.h"

void APlayerHUD::DrawHUD()
{
	Super::DrawHUD();

	FVector2D ViewportSize;
	if(GEngine) GEngine->GameViewport->GetViewportSize(ViewportSize);
	const FVector2D ViewportCenter(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);

	const float SpreadScaled = CrosshairSpreadMax * HUDPackage.CrosshairSpread;
	if(HUDPackage.CrosshairsCenter)
	{
		const FVector2D Spread(0.f, 0.f);
		DrawCrosshairs(HUDPackage.CrosshairsCenter, ViewportCenter, Spread, HUDPackage.Color);
	}
	if(HUDPackage.CrosshairsLeft)
	{
		const FVector2D Spread(-SpreadScaled, 0.f);
		DrawCrosshairs(HUDPackage.CrosshairsLeft, ViewportCenter, Spread, HUDPackage.Color);
	}
	if(HUDPackage.CrosshairsRight)
	{
		const FVector2D Spread(SpreadScaled, 0.f);
		DrawCrosshairs(HUDPackage.CrosshairsRight, ViewportCenter, Spread, HUDPackage.Color);
	}
	if(HUDPackage.CrosshairsTop)
	{
		const FVector2D Spread(0.f, -SpreadScaled);
		DrawCrosshairs(HUDPackage.CrosshairsTop, ViewportCenter, Spread, HUDPackage.Color);
	}
	if(HUDPackage.CrosshairsBottom)
	{
		const FVector2D Spread(0.f, SpreadScaled);
		DrawCrosshairs(HUDPackage.CrosshairsBottom, ViewportCenter, Spread, HUDPackage.Color);
	}
}

void APlayerHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("OverlayWidgetClass uninitialized, set in BP_PlayerHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("OverlayWidgetControlerClass uninitialized, set in BP_PlayerHUD"));
	
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UBaseUserWidget>(Widget);

	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);
	OverlayWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues();
	
	Widget->AddToViewport();
}

UOverlayWidgetController* APlayerHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if(OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
}

UAttributeMenuWidgetController* APlayerHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if(AttributeMenuWidgetController == nullptr)
	{
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParams(WCParams);
		AttributeMenuWidgetController->BindCallbacksToDependencies();
	}
	return AttributeMenuWidgetController;
}

/*
 * Crosshairs
 */
void APlayerHUD::DrawCrosshairs(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread, FLinearColor Color)
{
	const float TextureWidth = Texture->GetSizeX();
	const float TextureHeight = Texture->GetSizeY();
	const FVector2D TextureDrawPoint(
		ViewportCenter.X - (TextureWidth / 2.f) + Spread.X,
		ViewportCenter.Y - (TextureHeight / 2.f) + Spread.Y
	);

	DrawTexture(
		Texture, TextureDrawPoint.X, TextureDrawPoint.Y, TextureWidth, TextureHeight,
		0.f, 0.f, 1.f, 1.f, Color);
}
