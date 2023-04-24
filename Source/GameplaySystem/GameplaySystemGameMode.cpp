// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameplaySystemGameMode.h"
#include "GameplaySystemCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGameplaySystemGameMode::AGameplaySystemGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
