// Retropsis @ 2024


#include "UI/Player/OverheadWidget.h"

#include "Components/TextBlock.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerState.h"

void UOverheadWidget::NativeDestruct()
{
	RemoveFromParent();
	Super::NativeDestruct();
}

void UOverheadWidget::SetDisplayText(const FString& TextToDisplay) const
{
	if(DisplayText)
	{
		DisplayText->SetText(FText::FromString(TextToDisplay));
	}
}

void UOverheadWidget::ShowPlayerNetRole(const APawn* InPawn)
{
	const ENetRole RemoteRole = InPawn->GetRemoteRole();
	FString Role;
	switch (RemoteRole)
	{
	case ROLE_SimulatedProxy:
		Role = FString("SimulatedProxy");
		break;
	case ROLE_AutonomousProxy:
		Role = FString("AutonomousProxy");
		break;
	case ROLE_Authority:
		Role = FString("Authority");
		break;
	case ROLE_None:
		Role = FString("None");
		break;
	default: ;
	}
	const FString RemoteRoleString = FString::Printf(TEXT("Remote Role: %s"), *Role);
	SetDisplayText(RemoteRoleString);
}

void UOverheadWidget::ShowPlayerName(const ACharacter* InCharacter)
{
	if(InCharacter->GetPlayerState())
	{
		const FString PlayerNameString = FString::Printf(TEXT("%s"), *InCharacter->GetPlayerState()->GetPlayerName());
		SetDisplayText(PlayerNameString);
	}
}
