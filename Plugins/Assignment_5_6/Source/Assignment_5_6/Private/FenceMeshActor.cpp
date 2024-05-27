// Fill out your copyright notice in the Description page of Project Settings.


#include "FenceMeshActor.h"

// Sets default values
AFenceMeshActor::AFenceMeshActor()
{

	PrimaryActorTick.bCanEverTick = true;


	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	SplineComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);


}



void AFenceMeshActor::ClearStaticFenceComponents()
{
	for (auto& MeshComponent : StaticVerticalRailArr)
	{
		if (IsValid(MeshComponent))
			MeshComponent->DestroyComponent();
	}
	StaticVerticalRailArr.Empty();

}

void AFenceMeshActor::AddStaticFenceComponents()
{
	const float SplineLength = SplineComponent->GetSplineLength();

	const int NumberOfSplinePoints = SplineComponent->GetNumberOfSplinePoints();

	UE_LOG(LogTemp, Warning, TEXT("no of splin epoints %d amd spline length is %f "), NumberOfSplinePoints, SplineLength);

	for (int32 i{}; i < NumberOfSplinePoints; ++i)
	{
		SplineComponent->SetSplinePointType(i, ESplinePointType::Linear);
	}

	const int VerticalMeshLength = (FenceProperties.length * 10)/100 + FenceProperties.spacing;

	const float NumberOfVerticalFences = FMath::Floor(SplineLength / VerticalMeshLength);

	for (int32 i{}; i < NumberOfVerticalFences; ++i)
	{
		 FVector Location = SplineComponent->GetLocationAtDistanceAlongSpline(i * VerticalMeshLength, ESplineCoordinateSpace::World);
		 Location.Z = Location.Z + (FenceProperties.height / 2);
		
		FRotator Rotation = SplineComponent->GetRotationAtDistanceAlongSpline(i * VerticalMeshLength, ESplineCoordinateSpace::World);

		UStaticMeshComponent* VerticalRailMesh = NewObject<UStaticMeshComponent>(this);

		VerticalRailMesh->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepWorldTransform);

		VerticalRailMesh->SetWorldLocationAndRotation(Location, Rotation);

		VerticalRailMesh->SetStaticMesh(StaticVerticalRailActor);

		VerticalRailMesh->SetRelativeScale3D({ FenceProperties.length / 10, FenceProperties.width / 10, FenceProperties.height / 100 });

		VerticalRailMesh->RegisterComponent();

		StaticVerticalRailArr.Add(VerticalRailMesh);
	}


}
void AFenceMeshActor::ReplaceStaticMeshesWithProceduralMesh()
{
	for (auto& MeshComponent : StaticVerticalRailArr)
	{
		MeshComponent->DestroyComponent();

		if (VerticalRailActor)
		{
			FVector Location = MeshComponent->GetComponentLocation();
			FRotator Rotation = MeshComponent->GetComponentRotation();
			auto VerticalRail = GetWorld()->SpawnActor<AVerticalRailActor>(VerticalRailActor, Location, Rotation);

			VerticalRail->SetActorRelativeScale3D({ FenceProperties.length / 10, FenceProperties.width / 10, FenceProperties.height / 100 });
			if (VerticalRail)
			{
				VerticalRail->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			}
		}
	}
	StaticVerticalRailArr.Empty();
}



// Called when the game starts or when spawned
void AFenceMeshActor::BeginPlay()
{
	Super::BeginPlay();
	ReplaceStaticMeshesWithProceduralMesh();
}
void AFenceMeshActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	/*generate static mesh on comnstruction and replace with procedural mesh in begin play*/
	ClearStaticFenceComponents();
	AddStaticFenceComponents();
}


//below function for do all things with procedural mesh in construction
void AFenceMeshActor::OnConstructionProcedural()
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

void AFenceMeshActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

