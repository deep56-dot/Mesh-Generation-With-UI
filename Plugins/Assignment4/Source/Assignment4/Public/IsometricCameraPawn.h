// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "InputModifiers.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "IsometricCameraPawn.generated.h"

UCLASS()
class ASSIGNMENT4_API AIsometricCameraPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AIsometricCameraPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void RotateCameraCounterClockwise();

	void RotateCameraClockwise();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "Camera")
	UCameraComponent* CameraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "Camera")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY()
	class UInputAction* ClockRotateAction;


	UPROPERTY()
	class UInputAction* AntiClockRotateAction;

	//UPROPERTY()
	//class UInputAction* RotateAction;

	UPROPERTY()
	class UInputMappingContext* PawnMapping;

	float RotationAngleIncrement = 45.0f;
	float TargetYaw = 0.0f;
	float RotationSpeed = 45.0f;
	float TotalRotation = 0.0f;
};
