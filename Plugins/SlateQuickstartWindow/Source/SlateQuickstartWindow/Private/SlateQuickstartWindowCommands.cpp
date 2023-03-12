// Copyright Epic Games, Inc. All Rights Reserved.

#include "SlateQuickstartWindowCommands.h"

#define LOCTEXT_NAMESPACE "FSlateQuickstartWindowModule"

void FSlateQuickstartWindowCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "SlateQuickstartWindow", "Bring up SlateQuickstartWindow window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
