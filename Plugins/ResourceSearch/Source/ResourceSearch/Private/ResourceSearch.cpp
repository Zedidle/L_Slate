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


#include "Brushes/SlateImageBrush.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"





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

TSharedRef<SGridPanel> FResourceSearchModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	TSharedPtr<SGridPanel> WidgetGrid = SNew(SGridPanel);

	int WidgetNum = 0, RowCount = 15, Cols = 3;
	auto NextSlot = [WidgetNum, RowCount, Cols](TSharedPtr<SGridPanel> Grid, const FText& InLabel) mutable -> SHorizontalBox::FScopedWidgetSlotArguments
	{
		TSharedRef<SHorizontalBox> HBox = SNew(SHorizontalBox);

		// Checkbox to show disabled state
		Grid->AddSlot((WidgetNum / RowCount) * Cols, WidgetNum % RowCount)
			.Padding(12.f)
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			[
				SNew(SCheckBox)
				.IsChecked_Lambda([HBox] { return HBox->IsEnabled() ? ECheckBoxState::Checked : ECheckBoxState::Unchecked; })
			.OnCheckStateChanged_Lambda([HBox](ECheckBoxState NewState) { HBox->SetEnabled(NewState == ECheckBoxState::Checked); })
			];

		// Add the Label
		Grid->AddSlot((WidgetNum / RowCount) * Cols + 1, WidgetNum % RowCount)
			.Padding(24.f, 16.f, 12.f, 16.f)
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Right)
			[
				SNew(STextBlock)
				.Font(FAppStyle::Get().GetFontStyle("NormalFont"))
			.ColorAndOpacity(FAppStyle::Get().GetSlateColor("Colors.White50"))
			.Text(InLabel)
			];

		//auto& Ret = Grid->AddSlot((WidgetNum / RowCount)*2 + 1, WidgetNum % RowCount)
		Grid->AddSlot((WidgetNum / RowCount) * Cols + 2, WidgetNum % RowCount)
			.Padding(12.f, 16.f, 12.f, 16.f)
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Fill)
			[

				HBox
			];

		++WidgetNum;

		SHorizontalBox::FScopedWidgetSlotArguments NewSlot = HBox->AddSlot();
		NewSlot.VAlign(VAlign_Center)
			.HAlign(HAlign_Fill)
			.AutoWidth();
		return MoveTemp(NewSlot);
	};

	auto LeftRightLabel = [](const FName& InIconName = FName(), const FText& InLabel = FText::GetEmpty(), const FName& InTextStyle = TEXT("ButtonText")) -> TSharedRef<SWidget>
	{
		TSharedPtr<SHorizontalBox> HBox = SNew(SHorizontalBox);
		float Space = InIconName.IsNone() ? 0.0f : 8.f;

		if (!InIconName.IsNone())
		{
			HBox->AddSlot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				[
					SNew(SImage)
					.ColorAndOpacity(FSlateColor::UseForeground())
				.Image(FAppStyle::Get().GetBrush(InIconName))
				];
		}

		if (!InLabel.IsEmpty())
		{
			HBox->AddSlot()
				.VAlign(VAlign_Center)
				.Padding(Space, 0.5f, 0.f, 0.f)  // Compensate down for the baseline since we're using all caps
				.AutoWidth()
				[
					SNew(STextBlock)
					.TextStyle(&FAppStyle::Get().GetWidgetStyle< FTextBlockStyle >(InTextStyle))
				.Justification(ETextJustify::Center)
				.Text(InLabel)
				];
		}

		return SNew(SBox).HeightOverride(16.f)[HBox.ToSharedRef()];
	};


		NextSlot(WidgetGrid, LOCTEXT("SButtonPrimaryExampleLabelRounded", "Primary Button"))
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(8.f, 0.0f)
		.VAlign(VAlign_Center)
		[

			SNew(SButton)
			.ButtonStyle(&FAppStyle::Get().GetWidgetStyle< FButtonStyle >("PrimaryButton"))
		.TextStyle(&FAppStyle::Get().GetWidgetStyle< FTextBlockStyle >("ButtonText"))
		[
			LeftRightLabel(NAME_None, LOCTEXT("Label", "Label"))
		]
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(8.f, 0.0f)
		.VAlign(VAlign_Center)
		[
			SNew(SButton)
			.ButtonStyle(&FAppStyle::Get().GetWidgetStyle< FButtonStyle >("PrimaryButton"))
		[
			LeftRightLabel("Icons.box-perspective", LOCTEXT("Label", "Label"))
		]
		]

	+ SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		.Padding(8.f, 0.0f)
		[
			SNew(SButton)
			.ButtonStyle(&FAppStyle::Get().GetWidgetStyle< FButtonStyle >("PrimaryButton"))
		.VAlign(VAlign_Center)
		[
			LeftRightLabel("Icons.box-perspective")
		]
		]
		];
	
		return WidgetGrid;

	SNew(SDockTab)
		//+ SEditableText::Slot()
		[
			// Put your tab content here!
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				//SNew(SEditableText)
				//SAssignNew(EditableTextPtr, SEditableText)
				// SSearchBox
				NextSlot(WidgetGrid, LOCTEXT("SSearchBoxLabel", "SSearchBox"))
				.FillWidth(1.0)
				[
					SNew(SSearchBox)
				]

				//.SetText(FText("Hello World"))
				//[
				//	SNew(SButton)
					//+ SHorizontalBox::Slot()
					//.VAlign(VAlign_Center)
					//.Padding(5, 0, 0, 0)
					//.FillWidth(1.0)
					//[
					//	SAssignNew(SearchBoxPtr, SAssetSearchBox)
					//	.HintText(this, &FResourceSearchModule::GetSearchAssetsHintText)
					//	.OnTextChanged(this, &FResourceSearchModule::OnSearchBoxChanged)
					//	.OnTextCommitted(this, &FResourceSearchModule::OnSearchBoxCommitted)
					//	.OnKeyDownHandler(this, &FResourceSearchModule::OnSearchKeyDown)
					//	.OnAssetSearchBoxSuggestionFilter(this, &FResourceSearchModule::OnAssetSearchSuggestionFilter)
					//	.OnAssetSearchBoxSuggestionChosen(this, &FResourceSearchModule::OnAssetSearchSuggestionChosen)
					//	.DelayChangeNotificationsWhileTyping(true)
					//	//.Visibility((Config != nullptr ? Config->bCanShowAssetSearch : true) ? EVisibility::Visible : EVisibility::Collapsed)
					//	//.AddMetaData<FTagMetaData>(FTagMetaData(TEXT("ContentBrowserSearchAssets")))
					//]
				//]
			]
			+ SVerticalBox::Slot()
			[
				SNew(SButton)
				.HAlign(HAlign_Center)
				.OnClicked(FOnClicked::CreateRaw(this, &FResourceSearchModule::TestClicked))
				[
					SNew(STextBlock)
					.Text(LOCTEXT("", "角色"))
				]
			]
			//+ SAssetSearchBox::Slot()
		];
}

FReply FResourceSearchModule::TestClicked()
{
	UE_LOG(LogTemp, Log, TEXT("Hello Button!"));
	//EditableTextPtr->SetText("Hello Button!");
	//TextBlockPtr->SetText("资源检索");

	return FReply::Handled();
}

FText FResourceSearchModule::GetSearchAssetsHintText() const
{
	return FText();
}

//void FResourceSearchModule::OnSearchBoxChanged(const FText& InSearchText)
//{
//}
//
//void FResourceSearchModule::OnSearchBoxCommitted(const FText& InSearchText, ETextCommit::Type CommitInfo)
//{
//}
//
//FReply FResourceSearchModule::OnSearchKeyDown(const FGeometry& Geometry, const FKeyEvent& InKeyEvent)
//{
//	return FReply();
//}
//
//void FResourceSearchModule::OnAssetSearchSuggestionFilter(const FText& SearchText, TArray<FAssetSearchBoxSuggestion>& PossibleSuggestions, FText& SuggestionHighlightText) const
//{
//}
//
//FText FResourceSearchModule::OnAssetSearchSuggestionChosen(const FText& SearchText, const FString& Suggestion) const
//{
//	return FText();
//}

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