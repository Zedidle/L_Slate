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
		+ SWrapBox::Slot()
		[
			ConstructIconsGallery()
		]
	];
}
FReply SContentTileWidget::TestClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Hello Button!"));
	
	//FString IconPath2 = FPaths::EngineDir() + "Content/Slate/Starship/Common/save-modified.svg";  // 打包会自动把图标包含进去？
	FString IconPath2 = FPaths::ProjectContentDir() + "ResourceSearchAssets";
	TUniquePtr< FSlateBrush > Image = TUniquePtr<FSlateVectorImageBrush>(new FSlateVectorImageBrush(IconPath2, FVector2D(ImageSize, ImageSize)));
	ImagePtr->SetImage(Image.Get()); // 图标好像没设置成功，如何动态设置图标

	return FReply::Handled();
}
FReply SContentTileWidget::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("Hello OnMouseButtonDown!"));
	return FReply::Handled().DetectDrag(SharedThis(this), EKeys::LeftMouseButton);
}
FReply SContentTileWidget::OnDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("Hello OnDragDetected!"));
	if (GEditor)
	{
		if (GEditor->GetEditorWorldContext().World())
		{
			UWorld* W = GEditor->GetEditorWorldContext().World();

		}
		if (IsValid(GEditor->EditorWorld))
		{
			UGameInstance* GI = GEditor->EditorWorld->GetGameInstance();
			//GI->GetWorld()->SpawnActor();
		}
	}
	//没有GetWorld？要怎么获得当前的Level？
	// 怎么做键鼠投影？
	return FReply::Handled();
}
void SContentTileWidget::OnDragEnter(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("Hello OnDragEnter!"));
}
void SContentTileWidget::OnDragLeave(const FDragDropEvent& DragDropEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("Hello OnDragLeave!"));
}
FReply SContentTileWidget::OnDragOver(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("Hello OnDragOver!"));
	return FReply::Handled();
}
FReply SContentTileWidget::OnDrop(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("Hello OnDrop!"));
	return FReply::Handled();
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
