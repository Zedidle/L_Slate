// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
//#include "Editor/EditorWidgets/Public/SAssetSearchBox.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SUserWidget.h"

#include "Misc/Attribute.h"
#include "Styling/SlateColor.h"
#include "Fonts/SlateFontInfo.h"
#include "Input/CursorReply.h"
#include "Input/Reply.h"
#include "Layout/Visibility.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SWidget.h"
#include "Styling/SlateTypes.h"
#include "Styling/CoreStyle.h"
#include "Styling/SlateWidgetStyleAsset.h"
#include "Framework/SlateDelegates.h"
#include "Framework/MultiBox/MultiBoxExtender.h"
#include "Framework/Text/TextLayout.h"
#include "Widgets/Text/ISlateEditableTextWidget.h"
#include "Widgets/Input/SEditableText.h"
#include "Framework/Text/PlainTextLayoutMarshaller.h"
#include "Widgets/Text/SlateEditableTextLayout.h"
#include "Types/ReflectionMetadata.h"
#include "Types/TrackedMetaData.h"

class FToolBarBuilder;
class FMenuBuilder;
//class SAssetSearchBox;






class FResourceSearchModule : public IModuleInterface, public TSharedFromThis<FResourceSearchModule>
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();

	FReply TestClicked();

	
private:

	void RegisterMenus();

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);



private:
	TSharedPtr<class FUICommandList> PluginCommands;

	TSharedPtr<class SEditableText> EditableTextPtr;
	TSharedPtr<class SGridPanel> GridPanelPtr;
	int Column = 5;
	int Num = 0;

	
};
