// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
//#include "Editor/EditorWidgets/Public/SAssetSearchBox.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

class FToolBarBuilder;
class FMenuBuilder;
//class SAssetSearchBox;

class FResourceSearchModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();
	
private:

	void RegisterMenus();

	TSharedRef<class SGridPanel> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);

	FReply TestClicked();




private:
	TSharedPtr<class FUICommandList> PluginCommands;

	//TSharedPtr<SAssetSearchBox> SearchBoxPtr;

	TSharedPtr<SEditableText> EditableTextPtr;
	TSharedPtr<STextBlock> TextBlockPtr;

	FText GetSearchAssetsHintText() const;
	//void OnSearchBoxChanged(const FText& InSearchText);
	//void OnSearchBoxCommitted(const FText& InSearchText, ETextCommit::Type CommitInfo);
	//FReply OnSearchKeyDown(const FGeometry& Geometry, const FKeyEvent& InKeyEvent);
	//void OnAssetSearchSuggestionFilter(const FText& SearchText, TArray<FAssetSearchBoxSuggestion>& PossibleSuggestions, FText& SuggestionHighlightText) const;
	//FText OnAssetSearchSuggestionChosen(const FText& SearchText, const FString& Suggestion) const;

};
