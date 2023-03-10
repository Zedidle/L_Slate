// Copyright Epic Games, Inc. All Rights Reserved.

#include "ResourceSearchCommands.h"

#define LOCTEXT_NAMESPACE "FResourceSearchModule"

void FResourceSearchCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "ResourceSearch", "Bring up ResourceSearch window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
