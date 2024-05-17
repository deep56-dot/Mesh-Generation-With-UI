// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstPersonPawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "InputModifiers.h"
#include "PawnAttributeAsset.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"

AFirstPersonPawn::AFirstPersonPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Sphere"));
	Capsule->SetCapsuleHalfHeight(90);
	Capsule->SetCapsuleRadius(40);
	SetRootComponent(Capsule);

	//Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	//Body->SetupAttachment(Capsule);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->TargetArmLength = 600.f;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));

	MoveScale = 1.0f;
	RotateScale = 50.0f;

	PawnAssetData = LoadObject<UPawnAttributeAsset>(nullptr, TEXT("/Script/Assignment_1_2.PawnAttributeAsset'/Assignment_1_2/DataAssets/FirstPerson.FirstPerson'"));
}

void AFirstPersonPawn::BeginPlay()
{
	Super::BeginPlay();
}

void AFirstPersonPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

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

void AFirstPersonPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PawnMappingContext = NewObject<UInputMappingContext>(this);

	MoveAction = NewObject<UInputAction>(this);
	MoveAction->ValueType = EInputActionValueType::Axis3D;
	KeyMap(PawnMappingContext, MoveAction, EKeys::W);
	KeyMap(PawnMappingContext, MoveAction, EKeys::S, true);
	KeyMap(PawnMappingContext, MoveAction, EKeys::A, true, true);
	KeyMap(PawnMappingContext, MoveAction, EKeys::D, false, true);
	KeyMap(PawnMappingContext, MoveAction, EKeys::Q, true, true, EInputAxisSwizzle::ZYX);
	KeyMap(PawnMappingContext, MoveAction, EKeys::E, false, true, EInputAxisSwizzle::ZYX);

	RotateAction = NewObject<UInputAction>(this);
	RotateAction->ValueType = EInputActionValueType::Axis2D;
	KeyMap(PawnMappingContext, RotateAction, EKeys::MouseY);
	KeyMap(PawnMappingContext, RotateAction, EKeys::MouseX, false, true);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	APlayerController* FPC = Cast<APlayerController>(Controller);

	check(EIC && FPC);

	EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFirstPersonPawn::Move);
	EIC->BindAction(RotateAction, ETriggerEvent::Triggered, this, &AFirstPersonPawn::Rotate);
	ULocalPlayer* LocalPlayer = FPC->GetLocalPlayer();
	check(LocalPlayer);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(PawnMappingContext, 0);
}


void AFirstPersonPawn::Move(const FInputActionValue& ActionValue)
{

	FVector Input = ActionValue.Get<FInputActionValue::Axis3D>();

	FVector Input2 = GetActorRotation().RotateVector(Input);


	AddMovementInput(Input2, MoveScale);

}

void AFirstPersonPawn::Rotate(const FInputActionValue& ActionValue)
{

	FRotator Input(ActionValue[0], ActionValue[1], ActionValue[2]);
	Input *= GetWorld()->GetDeltaSeconds() * RotateScale;

	Input += GetActorRotation();
	Input.Pitch = FMath::ClampAngle(Input.Pitch, -89.9f, 89.9f);
	Input.Roll = 0;

	SetActorRotation(Input);
}