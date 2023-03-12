// Copyright Epic Games, Inc. All Rights Reserved.

#include "ResourceSearch.h"
#include "ResourceSearchStyle.h"
#include "ResourceSearchCommands.h"
#include "LevelEditor.h"
#include "ToolMenus.h"

#include "HAL/FileManager.h"

#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"

#include "Animation/CurveSequence.h"
#include "Widgets/SBoxPanel.h"
#include "Styling/SlateTypes.h"
#include "Styling/CoreStyle.h"

#include "Types/SlateEnums.h"
#include "UObject/ReflectedTypeAccessors.h"
// #include "Types/SlateEnums.h"

// #include "SlateCore.h"
#include "Brushes/SlateRoundedBoxBrush.h"


#include "Layout/WidgetPath.h"
#include "SlateOptMacros.h"
#include "Framework/Application/MenuStack.h"
#include "Framework/Application/SlateApplication.h"
#include "Framework/Docking/TabManager.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SFxWidget.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/Layout/SSpacer.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Images/SSpinningImage.h"
#include "Widgets/Notifications/SProgressBar.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SHeader.h"
#include "Widgets/Layout/SGridPanel.h"
#include "Widgets/Layout/SUniformWrapPanel.h"
#include "Widgets/Input/SMenuAnchor.h"
#include "Widgets/Text/SInlineEditableTextBlock.h"
#include "Widgets/Text/SMultiLineEditableText.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SEditableText.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SComboButton.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Colors/SColorBlock.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Input/SSearchBox.h"
#include "Widgets/Input/SSpinBox.h"
#include "Widgets/Input/SSlider.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Input/SNumericEntryBox.h"
#include "Widgets/Input/SSegmentedControl.h"
#include "Widgets/Layout/SWrapBox.h"
#include "Widgets/SCanvas.h"


#include "Brushes/SlateImageBrush.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"

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

#include "SEditorBoxWidget.h"
#include "SContentTileWidget.h"


static const FName ResourceSearchTabName("ResourceSearch");

#define LOCTEXT_NAMESPACE "FResourceSearchModule"

void FResourceSearchModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FResourceSearchStyle::Initialize();
	FResourceSearchStyle::ReloadTextures();

	FResourceSearchCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FResourceSearchCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FResourceSearchModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FResourceSearchModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(ResourceSearchTabName, FOnSpawnTab::CreateRaw(this, &FResourceSearchModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FResourceSearchTabTitle", "ResourceSearch"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);

}

void FResourceSearchModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FResourceSearchStyle::Shutdown();

	FResourceSearchCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(ResourceSearchTabName);
}

TSharedRef<SDockTab> FResourceSearchModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	return SNew(SDockTab)
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Center)
		.AutoHeight()
		[
			SNew(STextBlock)
			.Text(LOCTEXT("资源检索", "资 源 检 索"))
		]

		+ SVerticalBox::Slot()
			.VAlign(VAlign_Fill)
			.HAlign(HAlign_Fill)
		[
			SNew(SScrollBox)
			.Orientation(Orient_Vertical)
			+ SScrollBox::Slot()
			.VAlign(VAlign_Fill)
			.HAlign(HAlign_Fill)
			[
				SAssignNew(GridPanelPtr, SGridPanel)
			]
		]
		+ SVerticalBox::Slot()
			.VAlign(VAlign_Center)
			.AutoHeight()
			[
				SNew(SEditorBoxWidget)
				.GridPanelPtr(GridPanelPtr) // 时机只能放在 SAssignNew(GridPanelPtr, SGridPanel) 后面
			]
	];
}

FReply FResourceSearchModule::TestClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Hello Button!"));
	GridPanelPtr->AddSlot(Num / Column, Num % Column)
	.VAlign(VAlign_Fill)
	.HAlign(HAlign_Fill)
	[
		SNew(SContentTileWidget)
	];
	++Num;
	return FReply::Handled();
}

void FResourceSearchModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(ResourceSearchTabName);
}

void FResourceSearchModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FResourceSearchCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FResourceSearchCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FResourceSearchModule, ResourceSearch)

