// Copyright Epic Games, Inc. All Rights Reserved.

#include "InteractionSystemGameMode.h"
#include "DSCharacter.h"
#include "UObject/ConstructorHelpers.h"

AInteractionSystemGameMode::AInteractionSystemGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
