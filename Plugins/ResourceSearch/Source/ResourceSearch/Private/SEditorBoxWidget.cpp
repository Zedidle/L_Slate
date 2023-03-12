// Fill out your copyright notice in the Description page of Project Settings.


#include "SEditorBoxWidget.h"
#include "SlateOptMacros.h"
#include "SContentTileWidget.h"
#include "Widgets\Layout\SGridPanel.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SEditorBoxWidget::Construct(const FArguments& InArgs)
{
	GridPanelPtr = InArgs._GridPanelPtr;
	ChildSlot
	[
		SNew(SEditableText)
		//.Text(LOCTEXT("资源检索", "输 入 关 键 字"))
		.Text(FText::FromString("Input Key Value"))
		.Justification(ETextJustify::Center)
		.OnTextChanged(this, &SEditorBoxWidget::OnFilterTextChanged)
	];
}
void SEditorBoxWidget::OnFilterTextChanged(const FText& SearchText)
{
	if (GridPanelPtr.IsValid())
	{
		TSharedPtr<SGridPanel> GPtr = GridPanelPtr.Pin();
		GPtr->AddSlot(Num % Column, Num / Column)
			.VAlign(VAlign_Fill)
			.HAlign(HAlign_Fill)
			[
				SNew(SContentTileWidget)
			];
		++Num;
	}

}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
