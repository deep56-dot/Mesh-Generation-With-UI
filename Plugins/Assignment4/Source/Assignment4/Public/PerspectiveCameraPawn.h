// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "InputModifiers.h"
#include <Components/CapsuleComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <GameFramework/FloatingPawnMovement.h>
#include <Camera/CameraComponent.h>
 
#include "PerspectiveCameraPawn.generated.h"

UCLASS()
class ASSIGNMENT4_API APerspectiveCameraPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APerspectiveCameraPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void KeyMap(UInputMappingContext* InputMappingContext, UInputAction* InputAction, FKey Key,
		bool bNegate,
		bool bSwizzle, EInputAxisSwizzle SwizzleOrder);
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(const struct FInputActionValue& ActionValue);

	void Rotate(const struct FInputActionValue& ActionValue);

	UPROPERTY(EditAnywhere)
	UCameraComponent* CameraComponent;


	/*UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Body;*/

	UPROPERTY(EditAnywhere)
	UFloatingPawnMovement* Movement;

	UPROPERTY(EditAnywhere)
	USceneComponent* SceneComponent;

	UPROPERTY()
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditAnywhere)
	UCapsuleComponent* Capsule;

	/** Mapping context used for pawn control. */
	UPROPERTY()
	class UInputMappingContext* PawnMappingContext;

	/** Action to update location. */
	UPROPERTY()
	class UInputAction* MoveAction;

	UPROPERTY()
	class UInputAction* RotateAction;

	/** Scale to apply to location input. */
	UPROPERTY(EditAnywhere)
	float MoveScale;


	UPROPERTY(EditAnywhere)
	float RotateScale;

};


