// Fill out your copyright notice in the Description page of Project Settings.


#include "OrthographicCameraPawn.h"

// Sets default values
AOrthographicCameraPawn::AOrthographicCameraPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComponent->TargetArmLength = 1000;
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetRelativeRotation(FRotator(-90, 0, 0));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->SetProjectionMode(ECameraProjectionMode::Orthographic);


	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Floating Movement"));

}

// Called when the game starts or when spawned
void AOrthographicCameraPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOrthographicCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
static void KeyMap(UInputMappingContext* InputMappingContext, UInputAction* InputAction, FKey Key,
	bool bNegate = false,
	bool bSwizzle = false, EInputAxisSwizzle SwizzleOrder = EInputAxisSwizzle::YXZ)
{
	FEnhancedActionKeyMapping& Mapping = InputMappingContext->MapKey(InputAction, Key);
	UObject* Outer = InputMappingContext->GetOuter();

	if (bNegate)
	{
		UInputModifierNegate* Negate = NewObject<UInputModifierNegate>(Outer);
		Mapping.Modifiers.Add(Negate);
	}

	if (bSwizzle)
	{
		UInputModifierSwizzleAxis* Swizzle = NewObject<UInputModifierSwizzleAxis>(Outer);
		Swizzle->Order = SwizzleOrder;
		Mapping.Modifiers.Add(Swizzle);
	}
}
// Called to bind functionality to input
void AOrthographicCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PawnMapping = NewObject<UInputMappingContext>(this);

	MoveAction = NewObject<UInputAction>(this);
	MoveAction->ValueType = EInputActionValueType::Axis2D;
	KeyMap(PawnMapping, MoveAction, EKeys::W);
	KeyMap(PawnMapping, MoveAction, EKeys::S, true);
	KeyMap(PawnMapping, MoveAction, EKeys::A, true, true);
	KeyMap(PawnMapping, MoveAction, EKeys::D, false, true);


	ZoomAction = NewObject<UInputAction>(this);
	ZoomAction->ValueType = EInputActionValueType::Axis1D;
	KeyMap(PawnMapping, ZoomAction, EKeys::MouseWheelAxis);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	APlayerController* TDPC = Cast<APlayerController>(Controller);

	check(EIC)


		EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AOrthographicCameraPawn::Move);
	EIC->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &AOrthographicCameraPawn::Zoom);


	ULocalPlayer* LocalPlayer = TDPC->GetLocalPlayer();
	check(LocalPlayer);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);


	Subsystem->AddMappingContext(PawnMapping, 0);

}

void AOrthographicCameraPawn::Move(const FInputActionValue& ActionValue)
{
	FVector value = ActionValue.Get<FInputActionValue::Axis3D>();
	value.Z = 0;
	AddMovementInput(value, 2);
}

void AOrthographicCameraPawn::Zoom(const FInputActionValue& ActionValue)
{

	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("zoom inout"));
	float ZoomValue = ActionValue.Get<float>();
	float NewOrthoWidth = CameraComponent->OrthoWidth - (ZoomValue * 50);
	CameraComponent->OrthoWidth = FMath::Clamp(NewOrthoWidth, 512.0f, 4096.0f); // Adjust these values to your desired min/max zoom levels
}