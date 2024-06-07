// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MeshDataAsset.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FMeshDataStruct
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UStaticMesh* MeshType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MinScale;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxScale;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MinRotation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxRotation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MinDistance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxDistance;
};

UCLASS()
class ASSIGNMENT_5_6_API UMeshDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMeshDataStruct> MeshDataArr;
};
