// Fill out your copyright notice in the Description page of Project Settings.


#include "SMeshSelectionScrollBox.h"
#include "SlateOptMacros.h"
#include <Brushes/SlateColorBrush.h>

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SMeshSelectionScrollBox::Construct(const FArguments& InArgs)
{
	ParentVBox = SNew(SVerticalBox);
	ScrollBox = SNew(SScrollBox).Orientation(EOrientation::Orient_Horizontal);
	MeshAsset = InArgs._InMeshAsset;
	MeshType = InArgs._InMeshType;
	FSlateColorBrush* ParentBrush = new FSlateColorBrush(FLinearColor(0.955974f, 0.871367f, 0.590619f, 1.0f));
	TSharedPtr<SBorder> Border = SNew(SBorder).BorderImage(ParentBrush)[
		ParentVBox.ToSharedRef()
	];
	ChildSlot
		[
			Border.ToSharedRef()
		];
	if (MeshType == EAssetType::MeshData)
		RefreshAssetMeshThubnail();
	else if (MeshType == EAssetType::MaterialData)
		RefreshAssetMaterialThubnail();
	else if (MeshType == EAssetType::TextureData)
		RefreshAssetTextureThubnail();
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SMeshSelectionScrollBox::RefreshAssetMeshThubnail()
{
	ScrollBox->ClearChildren();
	if (MeshAsset.IsValid()) {

		ParentVBox->AddSlot()
			.AutoHeight()
			.HAlign(HAlign_Center)
			.Padding(10)
			[
				SNew(STextBlock)
					.Text(FText::FromString("Mesh Data"))
					.ColorAndOpacity(FColor::FromHex("#0d3b66")).Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 25))
			];


		for (const auto& MeshData : MeshAsset->MeshDataArray) {
			TSharedPtr<SVerticalBox> VerticalBox = SNew(SVerticalBox);
			FSlateBrush* ThumbnailBrush = new FSlateBrush();
			ThumbnailBrush->SetResourceObject(MeshData.Thumbnail);
			ThumbnailBrush->SetImageSize(FVector2D(200));
			TSharedPtr<STextBlock> TextBlock = SNew(STextBlock).ColorAndOpacity(FColor::FromHex("#0d3b66")).Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 20));
			TextBlock->SetText(FText::FromString(MeshData.Name));
			TSharedPtr<SImage> ThumbnailImage = SNew(SImage).Image(ThumbnailBrush).Cursor(EMouseCursor::Hand).OnMouseButtonDown_Lambda([this, MeshData](const FGeometry& Geometry, const FPointerEvent& MouseEvent) {
				if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
					OnThumbnailSelected.ExecuteIfBound(MeshData);
					return FReply::Handled();
				}
				return FReply::Unhandled();
				});
			FSlateColorBrush* BorderBrush = new FSlateColorBrush(FLinearColor(0.904661f, 0.651406f, 0.111932f, 1.0f));

			TSharedPtr<SBorder> ImageBorder = SNew(SBorder).BorderImage(BorderBrush).Padding(10)[
				ThumbnailImage.ToSharedRef()
			];
			VerticalBox->AddSlot()[
				ImageBorder.ToSharedRef()
			];
			VerticalBox->AddSlot().HAlign(EHorizontalAlignment::HAlign_Center).AutoHeight().Padding(10)
				[
					TextBlock.ToSharedRef()
				];

			ScrollBox->AddSlot().Padding(10).VAlign(EVerticalAlignment::VAlign_Center)
				[
					VerticalBox.ToSharedRef()
				];
		}
		ParentVBox->AddSlot()[
			ScrollBox.ToSharedRef()
		];
	}
}

void SMeshSelectionScrollBox::RefreshAssetMaterialThubnail()
{
	ScrollBox->ClearChildren();
	if (MeshAsset.IsValid()) {
		ParentVBox->AddSlot()
			.AutoHeight()
			.HAlign(HAlign_Center)
			.Padding(10)
			[
				SNew(STextBlock)
					.Text(FText::FromString("Material Data"))
					.ColorAndOpacity(FColor::FromHex("#0d3b66")).Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 25))
			];


		for (const auto& MeshData : MeshAsset->MaterialDataArray) {
			TSharedPtr<SVerticalBox> VerticalBox = SNew(SVerticalBox);
			FSlateBrush* ThumbnailBrush = new FSlateBrush();
			ThumbnailBrush->SetResourceObject(MeshData.Thumbnail);
			ThumbnailBrush->SetImageSize(FVector2D(200));
			TSharedPtr<STextBlock> TextBlock = SNew(STextBlock).ColorAndOpacity(FColor::FromHex("#0d3b66")).Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 20));
			TextBlock->SetText(FText::FromString(MeshData.Name));
			TSharedPtr<SImage> ThumbnailImage = SNew(SImage).Image(ThumbnailBrush).Cursor(EMouseCursor::Hand).OnMouseButtonDown_Lambda([this, MeshData](const FGeometry& Geometry, const FPointerEvent& MouseEvent) {
				if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
					OnMaterialThubnailSelected.ExecuteIfBound(MeshData);
					return FReply::Handled();
				}
				return FReply::Unhandled();
				});
			FSlateColorBrush* BorderBrush = new FSlateColorBrush(FLinearColor(0.904661f, 0.651406f, 0.111932f, 1.0f));

			TSharedPtr<SBorder> ImageBorder = SNew(SBorder).BorderImage(BorderBrush).Padding(10)[
				ThumbnailImage.ToSharedRef()
			];
			VerticalBox->AddSlot()[
				ImageBorder.ToSharedRef()
			];
			VerticalBox->AddSlot().HAlign(EHorizontalAlignment::HAlign_Center).AutoHeight().Padding(10)
				[
					TextBlock.ToSharedRef()
				];

			ScrollBox->AddSlot().Padding(10).VAlign(EVerticalAlignment::VAlign_Center)
				[
					VerticalBox.ToSharedRef()
				];
		}
		ParentVBox->AddSlot()[
			ScrollBox.ToSharedRef()
		];
	}
}

void SMeshSelectionScrollBox::RefreshAssetTextureThubnail()
{
	ScrollBox->ClearChildren();
	if (MeshAsset.IsValid()) {
		ParentVBox->AddSlot()
			.AutoHeight()
			.HAlign(HAlign_Center)
			.Padding(10)
			[
				SNew(STextBlock)
					.Text(FText::FromString("Texture Data"))
					.ColorAndOpacity(FColor::FromHex("#0d3b66")).Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 25))
			];


		for (const auto& MeshData : MeshAsset->TextureDatalArray) {
			TSharedPtr<SVerticalBox> VerticalBox = SNew(SVerticalBox);
			FSlateBrush* ThumbnailBrush = new FSlateBrush();
			ThumbnailBrush->SetResourceObject(MeshData.Thumbnail);
			ThumbnailBrush->SetImageSize(FVector2D(200));
			TSharedPtr<STextBlock> TextBlock = SNew(STextBlock).ColorAndOpacity(FColor::FromHex("#0d3b66")).Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 20));
			TextBlock->SetText(FText::FromString(MeshData.Name));
			TSharedPtr<SImage> ThumbnailImage = SNew(SImage).Image(ThumbnailBrush).Cursor(EMouseCursor::Hand).OnMouseButtonDown_Lambda([this, MeshData](const FGeometry& Geometry, const FPointerEvent& MouseEvent) {
				if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
					OnTextureThubnailSelected.ExecuteIfBound(MeshData);
					return FReply::Handled();
				}
				return FReply::Unhandled();
				});
			FSlateColorBrush* BorderBrush = new FSlateColorBrush(FLinearColor(0.904661f, 0.651406f, 0.111932f, 1.0f));
			TSharedPtr<SBorder> ImageBorder = SNew(SBorder).BorderImage(BorderBrush).Padding(10)[
				ThumbnailImage.ToSharedRef()
			];

			VerticalBox->AddSlot()[
				ImageBorder.ToSharedRef()
			];
			VerticalBox->AddSlot().HAlign(EHorizontalAlignment::HAlign_Center).AutoHeight().Padding(10)
				[
					TextBlock.ToSharedRef()
				];

			ScrollBox->AddSlot().Padding(10).VAlign(EVerticalAlignment::VAlign_Center)
				[
					VerticalBox.ToSharedRef()
				];
		}
		ParentVBox->AddSlot()[
			ScrollBox.ToSharedRef()
		];
	}
}
