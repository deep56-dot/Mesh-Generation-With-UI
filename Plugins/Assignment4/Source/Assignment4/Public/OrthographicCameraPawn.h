// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "InputModifiers.h"
#include <GameFramework/SpringArmComponent.h>
#include <GameFramework/FloatingPawnMovement.h>
#include <Camera/CameraComponent.h>
#include "OrthographicCameraPawn.generated.h"

UCLASS()
class ASSIGNMENT4_API AOrthographicCameraPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AOrthographicCameraPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY()
	class UInputAction* MoveAction;


	UPROPERTY()
	class UInputAction* ZoomAction;

	UPROPERTY()
	class UInputAction* RotateAction;

	UPROPERTY()
	class UInputMappingContext* PawnMapping;

	UPROPERTY()
	UFloatingPawnMovement* Movement;

	UPROPERTY(EditAnywhere)
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere)
	UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditAnywhere)
	UCapsuleComponent* Capsule;

	void Move(const struct FInputActionValue& ActionValue);

	void Zoom(const struct FInputActionValue& ActionValue);

	void Rotate(const FInputActionValue& ActionValue);


	UFUNCTION()
	void KeyMap(UInputMappingContext* InputMappingContext, UInputAction* InputAction, FKey Key,
		bool bNegate,
		bool bSwizzle, EInputAxisSwizzle SwizzleOrder);

};
