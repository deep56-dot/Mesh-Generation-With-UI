// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OrthographicCameraPawn.h"
#include "GameFramework/Actor.h"
#include "IsometricCameraPawn.h"
#include "PerspectiveCameraPawn.h"
#include "GameFramework/PlayerController.h"
#include <Kismet/KismetMaterialLibrary.h>
#include "SMeshSelectionScrollBox.h"
#include "WallSpline.h"
#include "Delegates/Delegate.h" 
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
#include <MyWidget.h>
#include "InteractiveArchController.generated.h"

DECLARE_DELEGATE_OneParam(FWallDelegate, FString);

UCLASS()
class ASSIGNMENT4_API AInteractiveArchController : public APlayerController
{
	GENERATED_BODY()
	void KeyMap(UInputMappingContext* InputMappingContext, UInputAction* InputAction, FKey Key, bool bNegate, bool bSwizzle, EInputAxisSwizzle SwizzleOrder);
	virtual void SetupInputComponent() override;


protected:
	AInteractiveArchController();
	virtual void BeginPlay() override;

public:

	UFUNCTION()
	void Click();

	UFUNCTION()
	void HideVisiblity();

	UFUNCTION()
	void ChangeView();

	UFUNCTION(BlueprintCallable)
	void Switch();

	UPROPERTY()
	FVector HitLocation;


	UPROPERTY()
	FVector ViewHitLocation=FVector(50,50,100);

	UPROPERTY()
	bool bMyActor = false;

	UPROPERTY()
	APawn* CurrentPawn;

	UPROPERTY()
	int num;

	UPROPERTY()
	int cnt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMeshSelectionScrollBox* ScrollWidget;

	UPROPERTY()
	USelectionBox* SelectionWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMyWidget* MyWidgetInstance;


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
	TSubclassOf<UMyWidget> MyWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AArchMeshActor* StaticMeshActor;

	UPROPERTY()
	UEnhancedInputLocalPlayerSubsystem* SubSystem;

	

	UPROPERTY()
	ULocalPlayer* LocalPlayer;

	UPROPERTY()
	UInputMappingContext* MyMapping;

	UPROPERTY()
	UInputMappingContext* CameraMapping;

	UPROPERTY()
	class UInputMappingContext* SplineMappingContext;

	/** Action to update location. */
	UPROPERTY()
	class UInputAction* LeftClickAction;

	UPROPERTY()
	class UInputAction* RightClickAction;

	UPROPERTY()
	class UInputAction* LeftArrow;

	UPROPERTY()
	class UInputAction* RightArrow;

	UPROPERTY()
	class UInputAction* UndoAction;

	UPROPERTY()
	TArray<AWallSpline*> WallSplineArr;

	UPROPERTY()
	int CurrWall;

	UPROPERTY()
	bool bAssignment3;

	UPROPERTY()
	APawn* SpawnedPawn;

	UFUNCTION(BlueprintCallable)
	void Delete();

	UFUNCTION(BlueprintCallable)
	void DeleteAll();

	void OnLeftClick(const struct FInputActionValue& ActionValue);
	void OnRightClick(const struct FInputActionValue& ActionValue);
	void Undo(const struct FInputActionValue& ActionValue);
	void OnLeft(const struct FInputActionValue& ActionValue);
	void OnRight(const struct FInputActionValue& ActionValue);


	FWallDelegate WallConstructionDelegate;

	UFUNCTION(BlueprintImplementableEvent)
	void ChangeText(const FString& text);
};
