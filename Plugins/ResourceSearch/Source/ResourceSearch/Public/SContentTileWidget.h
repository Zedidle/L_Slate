// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets\Input\SSearchBox.h"
#include "Brushes/SlateImageBrush.h"

const float ImageSize = 64.f;
// 远程更新的小图标
const FString IconPath = FPaths::EngineDir() + "Content/Slate/Starship/Common/SCC_Cloud_AtLatest.svg";  // 打包会自动把图标包含进去？
const TUniquePtr< FSlateBrush > CouldImage = TUniquePtr<FSlateVectorImageBrush>(new FSlateVectorImageBrush(IconPath, FVector2D(ImageSize / 4, ImageSize / 4)));

class RESOURCESEARCH_API SContentTileWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SContentTileWidget)
	    
    {}

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);


    TSharedPtr<SImage> ImagePtr;

    FReply TestClicked();


    TSharedPtr<SSearchBox> IconSearchBox;
    TArray< TUniquePtr< FSlateBrush > > DynamicBrushes;
    TSharedRef<SWidget> ConstructIconsGallery()
    {
        auto GenerateIconLibrarySVG = [this]() -> TSharedRef<SWidget>
        {
            const FVector2D IconSize(20.f, 20.f);
            TSharedPtr<SUniformWrapPanel> UniformWrapPanel = SNew(SUniformWrapPanel)
                .HAlign(HAlign_Left)
                .SlotPadding(FMargin(12.f, 12.f));

            TArray<FString> FoundIcons;
            //FString SearchDirectory = FPaths::EngineDir() / InPath;// TEXT("Editor/Slate/Icons/GeneralTools");
            FString SearchDirectory = FPaths::ProjectContentDir() + "ResourceSearchAssets";
            if ( ! IFileManager::Get().DirectoryExists(*SearchDirectory))
            {
                IFileManager::Get().MakeDirectory(*SearchDirectory);
            }

            // IFileManager::Get().FindFiles(FoundIcons, *SearchDirectory, TEXT(".png"));//, true, true, false);
            IFileManager::Get().FindFilesRecursive(FoundIcons, *SearchDirectory, TEXT("*.svg"), true, false);
            for (const FString& Filename : FoundIcons)
            {
                // FString IconPath = SearchDirectory / Filename;
                FString IconPath2 = Filename;

                DynamicBrushes.Add(TUniquePtr<FSlateVectorImageBrush>(new FSlateVectorImageBrush(IconPath2, IconSize)));

                UniformWrapPanel->AddSlot()
                    [
                        SNew(SImage)
                        .Image(DynamicBrushes.Last().Get())
                    .ToolTipText(FText::FromString(IconPath2))
                    .Visibility_Lambda([this, IconPath2]() { return EVisibility::Visible; })
                    ];
            }

            return SNew(SVerticalBox)
            + SVerticalBox::Slot()
                [
                    UniformWrapPanel.ToSharedRef()
                ];
        };



        return SNew(SBorder)
            .BorderImage(FAppStyle::Get().GetBrush("ToolPanel.GroupBorder"))
            [
                SNew(SScrollBox)
                + SScrollBox::Slot()
            .Padding(48)
            [
                SNew(SVerticalBox)

                + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(0.0f, 0.0f, 0.0f, 4.0f)
            [
                SAssignNew(IconSearchBox, SSearchBox)
                .HintText(FText::FromString("Enter text to filter icons by path..."))
            ]

            + SVerticalBox::Slot().AutoHeight()[GenerateIconLibrarySVG()]
            ]
        ];
    }


    virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
    virtual FReply OnDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
    virtual void OnDragEnter(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent) override;
    virtual void OnDragLeave(const FDragDropEvent& DragDropEvent) override;
    virtual FReply OnDragOver(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent) override;
    virtual FReply OnDrop(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent) override;

};
