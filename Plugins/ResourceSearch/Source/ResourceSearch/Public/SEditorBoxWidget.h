// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class RESOURCESEARCH_API SEditorBoxWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SEditorBoxWidget)
	: _GridPanelPtr()
	{}

	SLATE_ARGUMENT(TWeakPtr<class SGridPanel>, GridPanelPtr)


	SLATE_END_ARGS()

	int Num = 0;
	const int Column = 5;

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	void OnFilterTextChanged(const FText& SearchText);


private:
	TWeakPtr<class SGridPanel> GridPanelPtr;

};
