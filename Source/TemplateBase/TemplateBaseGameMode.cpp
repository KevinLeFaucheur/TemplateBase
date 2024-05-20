// Copyright Epic Games, Inc. All Rights Reserved.

#include "TemplateBaseGameMode.h"
#include "TemplateBaseCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATemplateBaseGameMode::ATemplateBaseGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
