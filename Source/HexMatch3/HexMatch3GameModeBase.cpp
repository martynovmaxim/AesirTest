// Copyright Epic Games, Inc. All Rights Reserved.


#include "HexMatch3GameModeBase.h"

AHexMatch3GameModeBase::AHexMatch3GameModeBase()
{
	PlayerControllerClass = AHexPlayerController::StaticClass();
	DefaultPawnClass = AHexPawn::StaticClass();
}
