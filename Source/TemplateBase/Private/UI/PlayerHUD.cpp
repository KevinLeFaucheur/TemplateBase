// Retropsis @ 2024


#include "UI/PlayerHUD.h"

void APlayerHUD::DrawHUD()
{
	Super::DrawHUD();

	FVector2D ViewportSize;
	if(GEngine) GEngine->GameViewport->GetViewportSize(ViewportSize);
	const FVector2D ViewportCenter(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);

	float SpreadScaled = CrosshairSpreadMax * HUDPackage.CrosshairSpread;
	if(HUDPackage.CrosshairsCenter)
	{
		const FVector2D Spread(0.f, 0.f);
		DrawCrosshairs(HUDPackage.CrosshairsCenter, ViewportCenter, Spread);
	}
	if(HUDPackage.CrosshairsLeft)
	{
		const FVector2D Spread(-SpreadScaled, 0.f);
		DrawCrosshairs(HUDPackage.CrosshairsLeft, ViewportCenter, Spread);
	}
	if(HUDPackage.CrosshairsRight)
	{
		const FVector2D Spread(SpreadScaled, 0.f);
		DrawCrosshairs(HUDPackage.CrosshairsRight, ViewportCenter, Spread);
	}
	if(HUDPackage.CrosshairsTop)
	{
		const FVector2D Spread(0.f, -SpreadScaled);
		DrawCrosshairs(HUDPackage.CrosshairsTop, ViewportCenter, Spread);
	}
	if(HUDPackage.CrosshairsBottom)
	{
		const FVector2D Spread(0.f, SpreadScaled);
		DrawCrosshairs(HUDPackage.CrosshairsBottom, ViewportCenter, Spread);
	}
}

void APlayerHUD::DrawCrosshairs(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread)
{
	const float TextureWidth = Texture->GetSizeX();
	const float TextureHeight = Texture->GetSizeY();
	const FVector2D TextureDrawPoint(
		ViewportCenter.X - (TextureWidth / 2.f) + Spread.X,
		ViewportCenter.Y - (TextureHeight / 2.f) + Spread.Y
	);

	DrawTexture(
		Texture, TextureDrawPoint.X, TextureDrawPoint.Y, TextureWidth, TextureHeight,
		0.f, 0.f, 1.f, 1.f, FLinearColor::White);
}
