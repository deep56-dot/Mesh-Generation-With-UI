// Fill out your copyright notice in the Description page of Project Settings.


#include "FenceMeshActor.h"

// Sets default values
AFenceMeshActor::AFenceMeshActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	SplineComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	//SplineComponent->SetClosedLoop(false);
	SplineComponent->ClearSplinePoints();
	SplinePoints = 0;

}

// Called when the game starts or when spawned
void AFenceMeshActor::BeginPlay()
{
	Super::BeginPlay();
	
}



void AFenceMeshActor::OnConstruction(const FTransform& Transform)
{
	FenceProperties.ClampValues();

	for (int i = 0; i < VerticalRailArr.Num(); i++) {
		if (VerticalRailArr[i]) {
			VerticalRailArr[i]->Destroy();
			VerticalRailArr[i] = nullptr;
		}
	}

	VerticalRailArr.Empty();
	SplineComponent->ClearSplinePoints();

	SplinePoints = (FenceProperties.length *100) / (FenceProperties.spacing+(10));
	for (int i = 0; i < SplinePoints; ++i)
	{
		FVector Position= SplineComponent->GetComponentLocation(); 
		Position = { (i * FenceProperties.spacing) +Position.X,  Position.Y,Position.Z};
		
		SplineComponent->AddSplinePoint(Position, ESplineCoordinateSpace::World);
	}
	for (int32 i = 0; i < SplinePoints; ++i)
	{
		FVector Position = SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World);
		FRotator Rotation = SplineComponent->GetRotationAtSplinePoint(i, ESplineCoordinateSpace::World);

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		
		Position.Z = Position.Z + (FenceProperties.height / 2);
		AVerticalRailActor* VerticalRail = GetWorld()->SpawnActor<AVerticalRailActor>(VerticalRailActor, Position, Rotation, SpawnParams);
		VerticalRail->SetActorRelativeScale3D(FVector(FenceProperties.width / 100, FenceProperties.width / 100, FenceProperties.height / 100));
		if (VerticalRail)
		{
			VerticalRail->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			VerticalRailArr.Add(VerticalRail);
		}
	}
}

// Called every frame
void AFenceMeshActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

