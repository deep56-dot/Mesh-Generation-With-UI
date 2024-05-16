// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include <Kismet/KismetMaterialLibrary.h>
#include "SMeshSelectionScrollBox.h"
#include "MeshSelectionScrollBox.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include <Engine/StaticMeshActor.h>
#include "InputModifiers.h"
#include "MeshAssetManager.h"
#include "ArchMeshActor.h"
#include <SelectionBox.h>
#include "InteractiveArchController.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENT4_API AInteractiveArchController : public APlayerController
{
	GENERATED_BODY()
	virtual void SetupInputComponent() override;


protected:
	AInteractiveArchController();
	virtual void BeginPlay() override;

public:

	UFUNCTION()
	void Click();

	UFUNCTION()
	void HideVisiblity();

	UPROPERTY()
	FVector HitLocation;

	UPROPERTY()
	bool bMyActor = false;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMeshSelectionScrollBox* ScrollWidget;

	UPROPERTY()
	USelectionBox* SelectionWidgetInstance;

	UFUNCTION(BlueprintCallable)
	void SpawnActor(const FMeshData& MeshData);

	UFUNCTION(BlueprintCallable)
	void SetMaterial(const  FMaterialData& MeshData);

	UFUNCTION(BlueprintCallable)
	void SetTexture(const FTextureData& TextureData);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstanceDynamic* DynamicMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<USelectionBox> SelectionWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AArchMeshActor* StaticMeshActor;
};
