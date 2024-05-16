// Fill out your copyright notice in the Description page of Project Settings.


#include "IsometricCameraPawn.h"

// Sets default values
AIsometricCameraPawn::AIsometricCameraPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->SetRelativeRotation(FRotator(-45.0, 0.0f, 0.0f));
	SpringArmComponent->TargetArmLength = 700.0f;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

// Called when the game starts or when spawned
void AIsometricCameraPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AIsometricCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float CurrentYaw = SpringArmComponent->GetRelativeRotation().Yaw;
	float NewYaw = FMath::FInterpTo(CurrentYaw, TargetYaw, DeltaTime, RotationSpeed);
	FRotator NewRelativeRotation = FRotator(SpringArmComponent->GetRelativeRotation().Pitch, NewYaw, SpringArmComponent->GetRelativeRotation().Roll);
	SpringArmComponent->SetRelativeRotation(NewRelativeRotation);

	// Debug message to display rotation
	//FVector DebugLocation = GetActorLocation() + FVector(0.0f, 0.0f, 100.0f); // Adjust Z offset as needed
	//FString RotationString = FString::Printf(TEXT("Spring Arm Rotation: %s"), *NewRelativeRotation.ToString());
	//DrawDebugString(GetWorld(), DebugLocation, RotationString, nullptr, FColor::White, DeltaTime);

}

// Called to bind functionality to input
void AIsometricCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("RotateClockwise", IE_Pressed, this, &AIsometricCameraPawn::RotateCameraClockwise);
	PlayerInputComponent->BindAction("RotateCounterClockwise", IE_Pressed, this, &AIsometricCameraPawn::RotateCameraCounterClockwise);
}

void AIsometricCameraPawn::RotateCameraCounterClockwise() {
	//SpringArmComponent->AddRelativeRotation(FRotator(0.0f, RotationAngleIncrement, 0.0f));
	TargetYaw += RotationAngleIncrement;
	if (TargetYaw > 180.0f) {
		TargetYaw = -180.0f + (TargetYaw - 180.0f);
	}

}
void AIsometricCameraPawn::RotateCameraClockwise() {
	//SpringArmComponent->AddRelativeRotation(FRotator(0.0f, -RotationAngleIncrement, 0.0f));

	TargetYaw -= RotationAngleIncrement;

	if (TargetYaw < -180.0f) {
		TargetYaw = 180.0f + (TargetYaw + 180.0f);
	}

}
