// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MeshAssetManager.h"
#include "InputCoreTypes.h"
#include "Widgets/SCompoundWidget.h"
#include <Widgets/Layout/SScrollBox.h>

/**
 * 
 */
DECLARE_DELEGATE_OneParam(FOnThubnailSelected, const FMeshData&);
DECLARE_DELEGATE_OneParam(FOnMaterialThubnailSelected, const FMaterialData&);
DECLARE_DELEGATE_OneParam(FOnTextureThubnailSelected, const FTextureData&);

UENUM()
enum class EAssetType {
	MeshData,
	MaterialData,
	TextureData
};
class ASSIGNMENT4_API SMeshSelectionScrollBox : public SCompoundWidget
{
public:
public:
	SLATE_BEGIN_ARGS(SMeshSelectionScrollBox) { }
		SLATE_ARGUMENT(TWeakObjectPtr<UMeshAssetManager>, InMeshAsset)
		SLATE_ARGUMENT(EAssetType, InMeshType)

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	FOnThubnailSelected OnThumbnailSelected;
	FOnMaterialThubnailSelected OnMaterialThubnailSelected;
	FOnTextureThubnailSelected OnTextureThubnailSelected;

private:
	void RefreshAssetMeshThubnail();
	void RefreshAssetMaterialThubnail();
	void RefreshAssetTextureThubnail();

	TWeakObjectPtr<UMeshAssetManager> MeshAsset;
	EAssetType MeshType;
	TSharedPtr<SScrollBox> ScrollBox;
	TSharedPtr<SVerticalBox> ParentVBox;
};
