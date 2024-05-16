// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "SMeshSelectionScrollBox.h"
#include "MeshSelectionScrollBox.generated.h"

/**
 * 
 */
DECLARE_DELEGATE_OneParam(FOnController, const FMeshData& MeshData);
DECLARE_DELEGATE_OneParam(FMaterialController, const FMaterialData& MeshData);
DECLARE_DELEGATE_OneParam(FTextureController, const FTextureData& MeshData);
UCLASS()
class ASSIGNMENT4_API UMeshSelectionScrollBox : public UWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeshData")
	UMeshAssetManager* MeshAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeshData")
	EAssetType MeshType = EAssetType::MeshData;


	FOnController OnMyController;
	FMaterialController MaterialController;
	FTextureController TextureController;

	UFUNCTION()
	void HandleAssetThubnailSelected(const FMeshData& MeshData);

	UFUNCTION()
	void HandleMaterialAssetThubnailSelected(const FMaterialData& MeshData);

	UFUNCTION()
	void HandleTextureAssetThubnailSelected(const FTextureData& MeshData);

	const FText GetPaletteCategory();
	void ReleaseSlateResources(bool bReleaseChildren);
protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
private:

	TSharedPtr<SMeshSelectionScrollBox> MeshSelectionScrollBox;

};
