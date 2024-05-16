// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshSelectionScrollBox.h"

TSharedRef<SWidget> UMeshSelectionScrollBox::RebuildWidget()
{
	MeshSelectionScrollBox = SNew(SMeshSelectionScrollBox).InMeshAsset(MeshAsset).InMeshType(MeshType);
	MeshSelectionScrollBox->OnThumbnailSelected.BindUObject(this, &UMeshSelectionScrollBox::HandleAssetThubnailSelected);
	MeshSelectionScrollBox->OnMaterialThubnailSelected.BindUObject(this, &UMeshSelectionScrollBox::HandleMaterialAssetThubnailSelected);
	MeshSelectionScrollBox->OnTextureThubnailSelected.BindUObject(this, &UMeshSelectionScrollBox::HandleTextureAssetThubnailSelected);

	return MeshSelectionScrollBox.ToSharedRef();
}

void UMeshSelectionScrollBox::HandleAssetThubnailSelected(const FMeshData& MeshData)
{
	OnMyController.ExecuteIfBound(MeshData);
}

void UMeshSelectionScrollBox::HandleMaterialAssetThubnailSelected(const FMaterialData& MeshData)
{
	MaterialController.ExecuteIfBound(MeshData);
}

void UMeshSelectionScrollBox::HandleTextureAssetThubnailSelected(const FTextureData& MeshData)
{
	TextureController.ExecuteIfBound(MeshData);
}

const FText UMeshSelectionScrollBox::GetPaletteCategory()
{
	return FText::FromString("Panel");
}

void UMeshSelectionScrollBox::ReleaseSlateResources(bool bReleaseChildren) {
	Super::ReleaseSlateResources(bReleaseChildren);

	MeshSelectionScrollBox.Reset();
}
