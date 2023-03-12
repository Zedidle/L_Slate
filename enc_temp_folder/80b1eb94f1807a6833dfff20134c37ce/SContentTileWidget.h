// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets\Input\SSearchBox.h"
#include "Brushes/SlateImageBrush.h"

/**
 * 
 */
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
        // auto GenerateColorButton = [] (FText InTitle, FName StyleColorName, bool Inverted = false) -> TSharedRef<SWidget> 

        auto GenerateIconLibrary = [this](FText InTitle, FString InPath) -> TSharedRef<SWidget>
        {
            const FVector2D IconSize(20.f, 20.f);
            TSharedPtr<SUniformWrapPanel> UniformWrapPanel = SNew(SUniformWrapPanel)
                .HAlign(HAlign_Left)
                .SlotPadding(FMargin(12.f, 12.f));

            TArray<FString> FoundIcons;
            FString SearchDirectory = FPaths::EngineDir() / InPath;// TEXT("Editor/Slate/Icons/GeneralTools");
            // IFileManager::Get().FindFiles(FoundIcons, *SearchDirectory, TEXT(".png"));//, true, true, false);
            IFileManager::Get().FindFilesRecursive(FoundIcons, *SearchDirectory, TEXT("*.png"), true, false);
            for (const FString& Filename : FoundIcons)
            {
                // FString IconPath = SearchDirectory / Filename;
                FString IconPath = Filename;

                DynamicBrushes.Add(TUniquePtr<FSlateDynamicImageBrush>(new FSlateDynamicImageBrush(FName(*IconPath), IconSize)));

                UniformWrapPanel->AddSlot()
                    [
                        SNew(SImage)
                        .Image(DynamicBrushes.Last().Get())
                        .ToolTipText(FText::FromString(IconPath))
                        .Visibility_Lambda([this, IconPath]() { return EVisibility::Visible; })
                    ];
            }

            return SNew(SVerticalBox)
                + SVerticalBox::Slot()
                .AutoHeight()
                [
                    SNew(STextBlock).Text(InTitle)
                ]

            + SVerticalBox::Slot()
                [
                    UniformWrapPanel.ToSharedRef()
                ];
        };

        auto GenerateIconLibrarySVG = [this](FText InTitle, FString InPath) -> TSharedRef<SWidget>
        {
            const FVector2D IconSize(20.f, 20.f);
            TSharedPtr<SUniformWrapPanel> UniformWrapPanel = SNew(SUniformWrapPanel)
                .HAlign(HAlign_Left)
                .SlotPadding(FMargin(12.f, 12.f));

            TArray<FString> FoundIcons;
            FString SearchDirectory = FPaths::EngineDir() / InPath;// TEXT("Editor/Slate/Icons/GeneralTools");
            // IFileManager::Get().FindFiles(FoundIcons, *SearchDirectory, TEXT(".png"));//, true, true, false);
            IFileManager::Get().FindFilesRecursive(FoundIcons, *SearchDirectory, TEXT("*.svg"), true, false);
            for (const FString& Filename : FoundIcons)
            {
                // FString IconPath = SearchDirectory / Filename;
                FString IconPath = Filename;

                DynamicBrushes.Add(TUniquePtr<FSlateVectorImageBrush>(new FSlateVectorImageBrush(IconPath, IconSize)));

                UniformWrapPanel->AddSlot()
                    [
                        SNew(SImage)
                        .Image(DynamicBrushes.Last().Get())
                    .ToolTipText(FText::FromString(IconPath))
                    .Visibility_Lambda([this, IconPath]() { return EVisibility::Visible; })
                    ];
            }

            return SNew(SVerticalBox)
                + SVerticalBox::Slot()
                .AutoHeight()
                [
                    SNew(STextBlock).Text(InTitle)
                ]

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

        + SVerticalBox::Slot().AutoHeight()[GenerateIconLibrarySVG(NSLOCTEXT("StarshipGallery", "SlateCore", "Core"), "Content/Slate/Starship/Common")]
            + SVerticalBox::Slot().AutoHeight()[GenerateIconLibrarySVG(NSLOCTEXT("StarshipGallery", "Editor Common", "Editor"), "Content/Editor/Slate/Starship/Common")]
            + SVerticalBox::Slot().AutoHeight()[GenerateIconLibrarySVG(NSLOCTEXT("StarshipGallery", "SceneOutliner", "SceneOutliner"), "Content/Editor/Slate/Starship/SceneOutliner")]
            + SVerticalBox::Slot().AutoHeight()[GenerateIconLibrarySVG(NSLOCTEXT("StarshipGallery", "GraphEditors", "GraphEditors"), "Content/Editor/Slate/Starship/GraphEditors")]
            + SVerticalBox::Slot().AutoHeight()[GenerateIconLibrarySVG(NSLOCTEXT("StarshipGallery", "MainToolbar", "MainToolbar"), "Content/Editor/Slate/Starship/MainToolbar")]
            +SVerticalBox::Slot().AutoHeight()[GenerateIconLibrary(NSLOCTEXT("StarshipGallery", "PaintIconTitle", "Paint"), "Content/Editor/Slate/Icons/Paint")]
            + SVerticalBox::Slot().AutoHeight()[GenerateIconLibrary(NSLOCTEXT("StarshipGallery", "LandscapeIconTitle", "Landscape"), "Content/Editor/Slate/Icons/Landscape")]
            + SVerticalBox::Slot().AutoHeight()[GenerateIconLibrary(NSLOCTEXT("StarshipGallery", "ModelingIconTitle", "Modeling"), "/Plugins/Editor/ModelingToolsEditorMode/Content/Icons")]
            + SVerticalBox::Slot().AutoHeight()[GenerateIconLibrary(NSLOCTEXT("StarshipGallery", "FractureIconTitle", "Fracture"), "/Plugins/Experimental/ChaosEditor/Content")]
            + SVerticalBox::Slot().AutoHeight()[GenerateIconLibrary(NSLOCTEXT("StarshipGallery", "CurveEditorIconTitle", "CurveEditor"), "Content/Editor/Slate/GenericCurveEditor/Icons")]
            + SVerticalBox::Slot().AutoHeight()[GenerateIconLibrary(NSLOCTEXT("StarshipGallery", "GeneralIconTitle", "General"), "Content/Editor/Slate/Icons/GeneralTools")]
            + SVerticalBox::Slot().AutoHeight()[GenerateIconLibrarySVG(NSLOCTEXT("StarshipGallery", "TimelineEditorIconTitle", "TimelineEditor"), "Content/Editor/Slate/Starship/TimelineEditor")]
            ]
            ];
    }


};
