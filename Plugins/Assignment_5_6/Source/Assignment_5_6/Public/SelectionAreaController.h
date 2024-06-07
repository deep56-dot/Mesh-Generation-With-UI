// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include <SelectionArea.h>
#include "SelectionAreaController.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENT_5_6_API ASelectionAreaController : public APlayerController
{
	GENERATED_BODY()
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ASelectionArea* SelectionActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bMove;
	
	

	ASelectionAreaController();

};
