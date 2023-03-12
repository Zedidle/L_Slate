// Fill out your copyright notice in the Description page of Project Settings.


#include "SContentTileWidget.h"
#include "SlateOptMacros.h"
#include "Widgets/Layout/SWrapBox.h"
#include "Brushes/SlateImageBrush.h"



BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SContentTileWidget::Construct(const FArguments& InArgs)
{

	ChildSlot
	[
		//ConstructIconsGallery()

		SNew(SWrapBox)
		+ SWrapBox::Slot()
		[
			SNew(SButton)
			.Text(FText::FromString("TEST"))
			.OnClicked(FOnClicked::CreateSP(this, &SContentTileWidget::TestClicked))
		]
		+ SWrapBox::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SAssignNew(ImagePtr, SImage)
			.Image(CouldImage.Get())
			.ToolTipText(FText::FromString(IconPath))
		]
		//+ SWrapBox::Slot()
		//.HAlign(HAlign_Left)
		//.VAlign(VAlign_Bottom)
		//[
		//	SNew(SImage)
		//	.Image(CouldImage.Get())
		//	.RenderTransform(FTransform2D(FVector2D ( -ImageSize / 4, 0)))
		//]
	];
}
FReply SContentTileWidget::TestClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Hello Button!"));
	FString IconPath2 = FPaths::EngineDir() + "Content/Slate/Starship/Common/save-modified.svg";  // 打包会自动把图标包含进去？
	TUniquePtr< FSlateBrush > Image = TUniquePtr<FSlateVectorImageBrush>(new FSlateVectorImageBrush(IconPath2, FVector2D(ImageSize, ImageSize)));
	ImagePtr->SetImage(Image.Get()); // 图标好像没设置成功

	return FReply::Handled();
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
