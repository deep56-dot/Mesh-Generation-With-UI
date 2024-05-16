// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MeshSelectionScrollBox.h"
#include <Components/CanvasPanel.h>
#include "SelectionBox.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENT4_API USelectionBox : public UUserWidget
{
	GENERATED_BODY()
protected:

	virtual void NativeConstruct() override;
public:
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CanvasPanel;
	UPROPERTY(meta = (BindWidget))
	UMeshSelectionScrollBox* MeshSelectionScrollBoxWidget;
	UPROPERTY(meta = (BindWidget))
	UMeshSelectionScrollBox* MaterialSelectionScrollBoxWidget;
	UPROPERTY(meta = (BindWidget))
	UMeshSelectionScrollBox* TextureSelectionScrollBoxWidget;
};
