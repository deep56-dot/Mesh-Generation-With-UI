// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MeshAssetManager.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FMeshData {

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMesh* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UTexture2D* Thumbnail;
};

USTRUCT(BlueprintType)
struct FMaterialData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	UMaterial* Material;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	UTexture2D* Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material", meta = (ClampMin = 0, ClampMax = 1))
	float Blurness;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material", meta = (ClampMin = 0, ClampMax = 1))
	float Roughness;
};

USTRUCT(BlueprintType)
struct FTextureData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture")
	UTexture2D* Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture")
	UTexture* Texture;
};

UCLASS()
class ASSIGNMENT4_API UMeshAssetManager : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TArray<FMeshData> MeshDataArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	TArray<FMaterialData> MaterialDataArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture")
	TArray<FTextureData> TextureDatalArray;

};
