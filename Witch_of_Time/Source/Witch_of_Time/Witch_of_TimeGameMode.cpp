// Copyright Epic Games, Inc. All Rights Reserved.

#include "Witch_of_TimeGameMode.h"
#include "Witch_of_TimeCharacter.h"
#include "UObject/ConstructorHelpers.h"

AWitch_of_TimeGameMode::AWitch_of_TimeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
