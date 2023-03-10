// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "ResourceSearchStyle.h"

class FResourceSearchCommands : public TCommands<FResourceSearchCommands>
{
public:

	FResourceSearchCommands()
		: TCommands<FResourceSearchCommands>(TEXT("ResourceSearch"), NSLOCTEXT("Contexts", "ResourceSearch", "ResourceSearch Plugin"), NAME_None, FResourceSearchStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};