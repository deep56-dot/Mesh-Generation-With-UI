// Fill out your copyright notice in the Description page of Project Settings.


#include "StaircaseActor.h"

// Sets default values
AStaircaseActor::AStaircaseActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root Scene"));
	RootComponent = RootScene;


	NoOfSteps = { 1 };
	length = { 0.5 };
	width = { 2 };
	depth = { 0.5 };
	Railing = { true };
	spacing = { 1.2 };
	TypeOfStair = StairType::Close;

}

// Called when the game starts or when spawned
void AStaircaseActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AStaircaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStaircaseActor::OnConstruction(const FTransform& Transform)
{

	FMesh::DestroyMeshCompomnent(StructArr);

	for (int i = 0; i < NoOfSteps; i++) {

		FMesh ele;
		FString StName = "StairCaseMeshCompnenet" + FString::FromInt(i);
		UStaticMeshComponent* StaircaseMeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *StName);
		if (StaircaseMeshComponent) {
			StaircaseMeshComponent->AttachToComponent(RootScene, FAttachmentTransformRules::KeepRelativeTransform);
			StaircaseMeshComponent->RegisterComponentWithWorld(GetWorld());
			StaircaseMeshComponent->SetStaticMesh(StaircaseMesh);

			if (TypeOfStair == StairType::Close)
			{
				StaircaseMeshComponent->SetRelativeLocation(FVector(length * 100 * i, 0, depth * 100 * i));
				StaircaseMeshComponent->SetRelativeScale3D(FVector(length, width, depth));

			}
			else if (TypeOfStair == StairType::Open)
			{
				StaircaseMeshComponent->SetRelativeLocation(FVector(length * 100 * i, 0, depth * 100 * i * spacing));

				StaircaseMeshComponent->SetRelativeScale3D(FVector(length, width, depth));

			}

			else {

				StaircaseMeshComponent->SetRelativeLocation(FVector(length * 100 * (i + 1), 0, 0));
				StaircaseMeshComponent->SetRelativeScale3D(FVector(length, width, (depth * (i + 1))));

			}
			ele.Stair = StaircaseMeshComponent;

		}


		if (Railing) {
			FString LName = "LeftRailMeshComponent" + FString::FromInt(i);
			UStaticMeshComponent* LeftRailMeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *LName);
			if (LeftRailMeshComponent) {
				LeftRailMeshComponent->AttachToComponent(StaircaseMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
				LeftRailMeshComponent->RegisterComponentWithWorld(GetWorld());
				LeftRailMeshComponent->SetStaticMesh(RailingMesh);
				LeftRailMeshComponent->SetMaterial(0, RailingMaterial);
				LeftRailMeshComponent->SetRelativeLocation(FVector(0, -50, 0));

				if (TypeOfStair == StairType::Box)
				{
					LeftRailMeshComponent->SetRelativeLocation(FVector(0, -50, 100));
					LeftRailMeshComponent->SetWorldScale3D(FVector(2.3 * length, 0.03, 0.45));
				}
				else
					LeftRailMeshComponent->SetRelativeScale3D(FVector(2, 0.03, 0.7));

				ele.Left = LeftRailMeshComponent;

			}

			FString RName = "RightRailMeshComponent" + FString::FromInt(i);
			UStaticMeshComponent* RightRailMeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), *RName);
			if (RightRailMeshComponent) {
				RightRailMeshComponent->AttachToComponent(StaircaseMeshComponent, FAttachmentTransformRules::KeepRelativeTransform);
				RightRailMeshComponent->RegisterComponentWithWorld(GetWorld());
				RightRailMeshComponent->SetStaticMesh(RailingMesh);
				RightRailMeshComponent->SetMaterial(0, RailingMaterial);
				RightRailMeshComponent->SetRelativeLocation(FVector(0, 50, 0));

				if (TypeOfStair == StairType::Box)
				{
					RightRailMeshComponent->SetRelativeLocation(FVector(0, 50, 100));
					RightRailMeshComponent->SetWorldScale3D(FVector(2.3 * length, 0.03, 0.45));

				}
				else
					RightRailMeshComponent->SetRelativeScale3D(FVector(2, 0.03, 0.7));

				ele.Right = RightRailMeshComponent;

			}
		}
		StructArr.Add(ele);
	}
}

void FMesh::DestroyMeshCompomnent(TArray<FMesh> StructArr_)
{
	for (int i = 0; i < StructArr_.Num(); i++) {

		if (StructArr_[i].Stair) {
			if (i > 0)
				UE_LOG(LogTemp, Warning, TEXT("%d and %f"), (i), StructArr_[i].Stair->GetRelativeScale3D().Z + 0.3);
			StructArr_[i].Stair->DestroyComponent();
			StructArr_[i].Stair = nullptr;
		}
		if (StructArr_[i].Left) {
			StructArr_[i].Left->DestroyComponent();
			StructArr_[i].Left = nullptr;
		}
		if (StructArr_[i].Right) {
			StructArr_[i].Right->DestroyComponent();
			StructArr_[i].Right = nullptr;
		}
	}
	StructArr_.Empty();
}
