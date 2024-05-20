// Fill out your copyright notice in the Description page of Project Settings.


#include "IsometricCameraPawn.h"

// Sets default values
AIsometricCameraPawn::AIsometricCameraPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->SetRelativeLocation(FVector(0,0,300));
	SpringArmComponent->SetRelativeRotation(FRotator(-45.0, 0.0f, 0.0f));
	SpringArmComponent->TargetArmLength = 2000.0f;

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
	/*Super::Tick(DeltaTime);

	float CurrentYaw = SpringArmComponent->GetRelativeRotation().Yaw;
	float NewYaw = FMath::FInterpTo(CurrentYaw, TargetYaw, DeltaTime, RotationSpeed);
	FRotator NewRelativeRotation = FRotator(SpringArmComponent->GetRelativeRotation().Pitch, NewYaw, SpringArmComponent->GetRelativeRotation().Roll);
	SpringArmComponent->SetRelativeRotation(NewRelativeRotation);*/

	Super::Tick(DeltaTime);

	FRotator CurrentRotation = SpringArmComponent->GetRelativeRotation();
	FRotator TargetRotation = FRotator(CurrentRotation.Pitch, TargetYaw, CurrentRotation.Roll);
	SpringArmComponent->SetRelativeRotation(FMath::Lerp(CurrentRotation, TargetRotation, DeltaTime));

}

// Called to bind functionality to input
void AIsometricCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	

	PawnMapping = NewObject<UInputMappingContext>(this);

	ClockRotateAction = NewObject<UInputAction>(this);
	ClockRotateAction->ValueType = EInputActionValueType::Boolean;
	PawnMapping->MapKey(ClockRotateAction, EKeys::Q);

	AntiClockRotateAction = NewObject<UInputAction>(this);
	AntiClockRotateAction->ValueType = EInputActionValueType::Boolean;
	PawnMapping->MapKey(AntiClockRotateAction, EKeys::E);

	

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	APlayerController* TDPC = Cast<APlayerController>(Controller);

	check(EIC)


		EIC->BindAction(ClockRotateAction, ETriggerEvent::Completed, this, &AIsometricCameraPawn::RotateCameraClockwise);
	    EIC->BindAction(AntiClockRotateAction, ETriggerEvent::Completed, this, &AIsometricCameraPawn::RotateCameraCounterClockwise);



	ULocalPlayer* LocalPlayer = TDPC->GetLocalPlayer();
	check(LocalPlayer);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);


	Subsystem->AddMappingContext(PawnMapping, 0);
}

void AIsometricCameraPawn::RotateCameraCounterClockwise() {

	TargetYaw += RotationAngleIncrement;
	if (TargetYaw > 180.0f) {
		TargetYaw = -180.0f + (TargetYaw - 180.0f);
	}

}
void AIsometricCameraPawn::RotateCameraClockwise() {

	TargetYaw -= RotationAngleIncrement;

	if (TargetYaw < -180.0f) {
		TargetYaw = 180.0f + (TargetYaw + 180.0f);
	}

}
