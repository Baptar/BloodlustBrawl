// Copyright Epic Games, Inc. All Rights Reserved.

#include "BloodlustBrawlGameMode.h"
#include "BloodlustBrawlCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABloodlustBrawlGameMode::ABloodlustBrawlGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
