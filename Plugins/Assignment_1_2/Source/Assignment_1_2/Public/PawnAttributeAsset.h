// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PawnAttributeAsset.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENT_1_2_API UPawnAttributeAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "100", UIMin = "0", UIMax = "100"))
	int Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float JumpHeight;
	
};
