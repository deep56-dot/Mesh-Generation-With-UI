// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectionArea.h"

// Sets default values
ASelectionArea::ASelectionArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ProceduralMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
	RootComponent = ProceduralMeshComponent;

	/*TranslucentMaterial = LoadObject<UMaterial>(nullptr, TEXT("/Script/Engine.MaterialInstanceConstant'/Engine/VREditor/LaserPointer/TranslucentHoverMaterial.TranslucentHoverMaterial'"));*/
	/*tatic ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("/Script/Engine.Material'/Engine/EngineDebugMaterials/M_SimpleTranslucent.M_SimpleTranslucent'"));
	if (Material.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("Material Added"));
		TranslucentMaterial = Material.Object;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Material NOt Added"));
		
	}*/
	bUseSphere = false;
}

// Called when the game starts or when spawned
void ASelectionArea::BeginPlay()
{

	GenerateCube(FVector(200));
	//GenerateSphere(100, 50, 50);
}
void ASelectionArea::ToggleShape()
{
	if (bUseSphere)
		GenerateSphere(100, 50, 50);
	else
		GenerateCube(FVector(200));
}
// Called every frame
void ASelectionArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASelectionArea::ChangeDimentions(FVector Dimensions)
{
	

		SetActorRelativeScale3D(Dimensions / 100);
		FVector Location = GetActorLocation();
		Location.Z = (Dimensions.Z );
		SetActorLocation(Location);
	
}


void ASelectionArea::GenerateCube(FVector Dimensions)
{
	ProceduralMeshComponent->ClearAllMeshSections();
	TArray<FVector> Vertices;

	float L = Dimensions.X / 2.0f;
	float W = Dimensions.Y / 2.0f;
	float H = Dimensions.Z / 2.0f;

	//Front Face
	Vertices.Add(FVector(-L, -W, -H)); //0
	Vertices.Add(FVector(-L, W, -H)); //1
	Vertices.Add(FVector(-L, W, H)); //2
	Vertices.Add(FVector(-L, -W, H)); //3

	//Bottom Face
	Vertices.Add(FVector(-L, -W, -H)); //4
	Vertices.Add(FVector(L, -W, -H));  //5
	Vertices.Add(FVector(L, W, -H));   //6
	Vertices.Add(FVector(-L, W, -H));  //7

	//Back Face
	Vertices.Add(FVector(L, -W, -H)); //8
	Vertices.Add(FVector(L, -W, H)); //9
	Vertices.Add(FVector(L, W, H)); //10
	Vertices.Add(FVector(L, W, -H)); //11

	//Top Face
	Vertices.Add(FVector(L, -W, H)); //12
	Vertices.Add(FVector(-L, -W, H)); //13
	Vertices.Add(FVector(-L, W, H));  //14
	Vertices.Add(FVector(L, W, H));  //15

	//Left Face
	Vertices.Add(FVector(-L, -W, H)); //16
	Vertices.Add(FVector(L, -W, H)); //17
	Vertices.Add(FVector(L, -W, -H)); //18
	Vertices.Add(FVector(-L, -W, -H)); //19

	//Right Face
	Vertices.Add(FVector(-L, W, H)); //20
	Vertices.Add(FVector(-L, W, -H)); //21 
	Vertices.Add(FVector(L, W, -H)); //22
	Vertices.Add(FVector(L, W, H)); //23

	// Triangles

	TArray<int32> Triangles = { 0,1,3, 1,2,3, 4,5,7, 5,6,7 ,8,9,11, 9,10,11, 12,13,15, 13,14,15, 16,17,19, 17,18,19, 20,21,23, 21,22,23 };

	TArray<FVector2D>  UVs = { FVector2D(0,1), FVector2D(1,1), FVector2D(1,0), FVector2D(0,0),FVector2D(0,0),FVector2D(0,1),FVector2D(1,1),FVector2D(1,0),FVector2D(1,1),FVector2D(1,0),FVector2D(0,0),FVector2D(0,1),FVector2D(0,0),FVector2D(0,1),FVector2D(1,1),FVector2D(1,0),FVector2D(1,0),FVector2D(0,0),FVector2D(0,1),FVector2D(1,1), FVector2D(0,0),FVector2D(0,1),FVector2D(1,1),FVector2D(1,0) };

	TArray<FVector> Normals = {
		FVector(-1,0,0), FVector(-1,0,0), FVector(-1,0,0), FVector(-1,0,0),
		FVector(0,0,-1),FVector(0,0,-1),FVector(0,0,-1),FVector(0,0,-1),
		FVector(1, 0, 0), FVector(1, 0, 0), FVector(1, 0, 0), FVector(1, 0, 0),
		FVector(0,0,1) ,FVector(0,0,1),FVector(0,0,1),FVector(0,0,1),
		FVector(0,-1,0),FVector(0,-1,0),FVector(0,-1,0),FVector(0,-1,0),
		FVector(0,1,0),FVector(0,1,0),FVector(0,1,0),FVector(0,1,0)
	};
	ProceduralMeshComponent->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);

	ProceduralMeshComponent->SetMaterial(0, TranslucentMaterial);

}

void ASelectionArea::GenerateSphere(float Radius, int32 Segments, int32 Rings)
{
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FLinearColor> Colors;
	TArray<FVector2D> UVs;
	TArray<FProcMeshTangent> Tangents;


	Vertices.Empty();
	Triangles.Empty();
	Normals.Empty();
	UVs.Empty();
	Tangents.Empty();
	FVector Offset = FVector(0, 0, 0);

	for (int32 RingNumber = 0; RingNumber <= Rings; RingNumber++)
	{
		for (int32 SegmentNumber = 0; SegmentNumber <= Segments; SegmentNumber++)
		{
			float Theta = 2.0f * PI * (float)SegmentNumber / (float)Segments;
			float Phi = PI * (float)RingNumber / (float)Rings;

			float X = FMath::Sin(Phi) * FMath::Cos(Theta);
			float Y = FMath::Sin(Phi) * FMath::Sin(Theta);
			float Z = FMath::Cos(Phi);

			FVector Vertex = FVector(X, Y, Z) * Radius + Offset;
			Vertices.Add(Vertex);

			FVector Normal = Vertex.GetSafeNormal();
			Normals.Add(Normal);

			FVector2D UV = FVector2D((float)SegmentNumber / (float)Segments, (float)RingNumber / (float)Rings);
			UVs.Add(UV);

			FProcMeshTangent Tangent = FProcMeshTangent(1.f, 0.f, 0.f);
			Tangents.Add(Tangent);
		}
	}

	for (int32 RingNumber = 0; RingNumber < Rings; RingNumber++)
	{
		for (int32 SegmentNumber = 0; SegmentNumber < Segments; SegmentNumber++)
		{
			int32 FirstVertex = (RingNumber * (Segments + 1)) + SegmentNumber;
			int32 SecondVertex = FirstVertex + Segments + 1;

			Triangles.Add(FirstVertex);
			Triangles.Add(FirstVertex + 1);
			Triangles.Add(SecondVertex);

			Triangles.Add(SecondVertex);
			Triangles.Add(FirstVertex + 1);
			Triangles.Add(SecondVertex + 1);
		}
	}

	ProceduralMeshComponent->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, Colors, Tangents, true);
	ProceduralMeshComponent->SetMaterial(0, TranslucentMaterial);

}
