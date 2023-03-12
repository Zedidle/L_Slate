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
			//.Text(FText::FromString("资 源 检 索"))
		
		]
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Center)
		.AutoHeight()
		[
			SNew(SEditableText)
			.Text(LOCTEXT("资源检索", "输入关键字")) 
			//.Text(FText::FromString("输入资源关键字"))
			.OnTextChanged(this, &FResourceSearchModule::OnFilterTextChanged)
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
				+ SGridPanel::Slot(0, 0)
				.VAlign(VAlign_Fill)
				.HAlign(HAlign_Fill)
				[
					SNew(SButton)
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
				]
				+ SGridPanel::Slot(0, 1)
				.VAlign(VAlign_Fill)
				.HAlign(HAlign_Fill)
				[
					SNew(SButton)
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					.Text(FText::FromString("Press Me"))
					.OnClicked(FOnClicked::CreateSP(this, &FResourceSearchModule::TestClicked)) // 为什么绑定成功了点击还是没有反应？？？
				]
				+ SGridPanel::Slot(0, 2)
				.VAlign(VAlign_Fill)
				.HAlign(HAlign_Fill)
				[
					SNew(SContentTileWidget)
				]
				+ SGridPanel::Slot(1, 2)
				.VAlign(VAlign_Fill)
				.HAlign(HAlign_Fill)
				[
					SNew(SContentTileWidget)
				]
				+ SGridPanel::Slot(8, 2)
				.VAlign(VAlign_Fill)
				.HAlign(HAlign_Fill)
				[
					SNew(SContentTileWidget)
				]
			]
		]

	];




	// ======================



	//SNew(SWrapBox)
	//	.PreferredWidth(300.f)
	//	+ SWrapBox::Slot()
	//	.Padding(5)
	//	.VAlign(VAlign_Top)
	//	[
	//		SNew(STextBlock)
	//		.Text(LOCTEXT("资源检索", "资 源 检 索"))
	//	]




	//SNew(SCanvas)
	//	+ SCanvas::Slot().VAlign(VAlign_Center)
	//	+ SCanvas::Slot().HAlign(HAlign_Center)
	//	[
	//		SNew(SEditableText)
	//		.Text(LOCTEXT("CreateLocalCollection", "Create Local Collection"))
	//	]



	//SAssignNew(SearchBoxPtr, SSearchBox)
	//	.SelectAllTextWhenFocused(true)
	//	.OnTextChanged(this, &FResourceSearchModule::OnFilterTextChanged); // 为什么因为这行编译不过？

	//return SNew(SDockTab)
	//[
	//	//SNew(SButton)
	//	//SAssignNew(SearchBoxPtr, SSearchBox)
	//	//.SelectAllTextWhenFocused(true)
	//	//.OnTextChanged(this, &FResourceSearchModule::OnFilterTextChanged) // 为什么因为这行编译过没效果？

	//	SNew(SScrollBox)
	//	+ SScrollBox::Slot().Padding(10, 5)
	//	[
	//		SNew(SHorizontalBox)
	//		+ SHorizontalBox::Slot().HAlign(HAlign_Left)
	//		[
	//			SAssignNew(SearchBoxPtr, SSearchBox)
	//			.OnTextChanged(this, &FResourceSearchModule::OnFilterTextChanged) // 为什么因为这行编译过没效果？
	//		]
	//		+ SHorizontalBox::Slot().HAlign(HAlign_Center)
	//		[
	//			SNew(SSearchBox)
	//		]
	//		+ SHorizontalBox::Slot().HAlign(HAlign_Right)
	//		[
	//			SNew(SSearchBox)
	//		]
	//	]
	//	+ SScrollBox::Slot().Padding(10, 5)
	//		[
	//			SNew(SHorizontalBox)
	//			+ SHorizontalBox::Slot().FillWidth(2)
	//		[
	//			SNew(SSearchBox)
	//		]
	//		+ SHorizontalBox::Slot().FillWidth(1)
	//		[
	//			SNew(SSearchBox)
	//		]
	//		+ SHorizontalBox::Slot().FillWidth(3)
	//		[
	//			SNew(SSearchBox)
	//		]
	//	]


	//return SNew(SDockTab)
	//[
	//SNew(SUniformGridPanel)
	//	.SlotPadding(FMargin(5.0f))
	//	+ SUniformGridPanel::Slot(0, 0)
	//	.HAlign(HAlign_Right)
	//	[
	//		SNew(SSearchBox)
	//	]
	//+ SUniformGridPanel::Slot(0, 1)
	//	.HAlign(HAlign_Right)
	//	[
	//		SNew(SSearchBox)
	//	]
	//+ SUniformGridPanel::Slot(0, 2)
	//	.HAlign(HAlign_Center)
	//	[
	//		SNew(SSearchBox)
	//	]
	//+ SUniformGridPanel::Slot(1, 0)
	//	[
	//		SNew(SSearchBox)
	//	]
	//+ SUniformGridPanel::Slot(1, 1)
	//	[
	//		SNew(SSearchBox)
	//	]
	//];

	//return SNew(SDockTab)
	//[
	//	SNew(SWrapBox)
	//	.PreferredWidth(300.f)
	//	+ SWrapBox::Slot()
	//	.Padding(5)
	//	.VAlign(VAlign_Top)
	//	[
	//		SNew(SSearchBox)
	//	]
	//+ SWrapBox::Slot()
	//	.Padding(5)
	//	.VAlign(VAlign_Bottom)
	//	[
	//		SNew(SSearchBox)
	//	]
	//+ SWrapBox::Slot()
	//	.Padding(FMargin(20, 5, 0, 5))
	//	.VAlign(VAlign_Center)
	//	[
	//		SNew(SSearchBox)
	//	]
	//+ SWrapBox::Slot()
	//	.Padding(0)
	//	.VAlign(VAlign_Fill)
	//	[
	//		SNew(SSearchBox)
	//	]
	//];

}

FReply FResourceSearchModule::TestClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Hello Button!"));
	GridPanelPtr->AddSlot(Num / Column, Num % Column)
	[
		SNew(SImage)
		.Image(CouldImage.Get())
	];
	++Num;
	return FReply::Handled();
}

void FResourceSearchModule::OnFilterTextChanged(const FText& SearchText)
{
	UE_LOG(LogTemp, Log, TEXT("Hello OnFilterTextChanged!"));
}



FText FResourceSearchModule::GetSearchAssetsHintText() const
{
	return FText();
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

