// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PawnAttributeAsset.h"
#include <Components/SphereComponent.h>
#include "Components/CapsuleComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "FirstPersonPawn.generated.h"

UCLASS()
class ASSIGNMENT_1_2_API AFirstPersonPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AFirstPersonPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPawnAttributeAsset* PawnAssetData;

};
