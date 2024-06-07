// Fill out your copyright notice in the Description page of Project Settings.


#include "VerticalRailActor.h"
#include <Kismet/KismetMathLibrary.h>

// Sets default values
AVerticalRailActor::AVerticalRailActor() : Dimentions{ FVector(10,10,100) }
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProceduralMeshRoot = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
	RootComponent = ProceduralMeshRoot;

}

void AVerticalRailActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	ProceduralMeshRoot->ClearAllMeshSections();
	GenerateCube(Dimentions);
	GenerateTopHorizontalRail(Dimentions);
	GenerateBottomHorizontalRail(Dimentions);
	if(RailingType==ETypeOfRail::RoundTurnedCapital)
	{
		GenerateBellShape(FVector(0, 0, Dimentions.Z / 2), Dimentions.Y / 4, 2.7, Dimentions.Y / 8, 1, 10, 10);

		GenerateSphere(Dimentions.X / 2, 32, 32);
	}
	else if(RailingType==ETypeOfRail::GothicStarTop)
	{
		GeneratePyramid(Dimentions);
	}
	else if (RailingType == ETypeOfRail::ACornCapital)
	{
		GenerateBellShape(FVector(0, 0, Dimentions.Z / 2), Dimentions.Y / 4, 2, Dimentions.Y / 8, 1, 10, 10);
		GenerateEgg(Dimentions.X / 2, 32, 32);
	}
	else if (RailingType == ETypeOfRail::PyramidTop)
	{
		GenerateBellShape(FVector(0, 0, Dimentions.Z / 2), Dimentions.Y / 4, 1, Dimentions.Y / 8, 1, 10, 10);
		GenerateCubePyramid(Dimentions);

	}
	else if (RailingType == ETypeOfRail::WindsorTurnedCapital)
	{

		GenerateBellShape(FVector(0, 0, Dimentions.Z / 2), Dimentions.Y / 4, 2.7, Dimentions.Y / 8, 1, 10, 10);
		GeneratePyramidOverSphere(Dimentions);
		GenerateSphere(Dimentions.X / 2, 32, 32);
		
	}
	else if (RailingType == ETypeOfRail::RoundedOverTop)
	{

		GenerateBellShape(FVector(0, 0, Dimentions.Z / 2), Dimentions.Y / 4, 1, Dimentions.Y / 8, 1, 10, 10);
		CreateSemiDisk(Dimentions.X / 2, Dimentions.Y , FVector(0, 0, Dimentions.Z / 2+1));

	}
}

void AVerticalRailActor::GenerateBellShape(FVector Location, float BaseRadius, float Height1, float RimRadius, float CurvatureFactor, int NumSlices, int NumStacks)
{
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FProcMeshTangent> Tangents;
	TArray<FLinearColor> Colors;

	float Theta, Phi;
	float DeltaTheta = 2.0f * PI / NumSlices;
	float DeltaPhi = PI / (NumStacks - 1);

	// Generate vertices
	for (int i = 0; i < NumStacks; i++)
	{
		Phi = i * DeltaPhi;

		// Bell profile function for radius interpolation with curvature
		float t = Phi / (PI / 2.0f);
		float CurrentRadius = FMath::Lerp(BaseRadius, RimRadius, FMath::Sin(t * CurvatureFactor)) + 2.5;

		for (int j = 0; j < NumSlices; j++)
		{
			Theta = j * DeltaTheta;

			float X = CurrentRadius * FMath::Sin(Theta);
			float Y = CurrentRadius * FMath::Cos(Theta);
			float Z = Height1 * (t - 1.0f); // Z decreases with height for bell shape


			// Apply location offset
			X += Location.X;
			Y += Location.Y;
			Z += Location.Z;

			// Invert Z-coordinate


			Vertices.Add(FVector(X, Y, Z + Height1));

			// Calculate normals for lighting (approximate)
			Normals.Add(FVector(X, Y, Z).GetSafeNormal());

			// UV mapping
			UVs.Add(FVector2D((float)j / (NumSlices - 1), (float)i / (NumStacks - 1)));

			// Tangents (optional)
			Tangents.Add(FProcMeshTangent(-FMath::Sin(Theta), FMath::Cos(Theta), 0.0f));
		}
	}

	// Generate triangles (quads converted to triangles)
	for (int i = 0; i < NumStacks - 1; i++)
	{
		for (int j = 0; j < NumSlices - 1; j++)
		{
			int BottomLeft = i * NumSlices + j;
			int BottomRight = BottomLeft + 1;
			int TopLeft = BottomLeft + NumSlices;
			int TopRight = TopLeft + 1;

			// First triangle (bottom left to top left to top right)
			Triangles.Add(BottomLeft);
			Triangles.Add(TopLeft);
			Triangles.Add(BottomRight);

			Triangles.Add(BottomLeft);
			Triangles.Add(BottomRight);
			Triangles.Add(TopLeft);

			// Second triangle (top left to top right to bottom right)
			Triangles.Add(TopLeft);
			Triangles.Add(TopRight);
			Triangles.Add(BottomRight);

			Triangles.Add(TopLeft);
			Triangles.Add(BottomRight);
			Triangles.Add(TopRight);
		}

		// Handle wrap-around triangle for each stack
		int BottomLeftWrap = i * NumSlices + (NumSlices - 1);
		int BottomRightWrap = i * NumSlices;
		int TopLeftWrap = BottomLeftWrap + NumSlices;
		int TopRightWrap = BottomRightWrap + NumSlices;

		// First triangle (bottom left to top left to top right)
		Triangles.Add(BottomLeftWrap);
		Triangles.Add(TopLeftWrap);
		Triangles.Add(BottomRightWrap);

		Triangles.Add(BottomLeftWrap);
		Triangles.Add(BottomRightWrap);
		Triangles.Add(TopLeftWrap);

		// Second triangle (top left to top right to bottom right)
		Triangles.Add(TopLeftWrap);
		Triangles.Add(TopRightWrap);
		Triangles.Add(BottomRightWrap);

		Triangles.Add(TopLeftWrap);
		Triangles.Add(BottomRightWrap);
		Triangles.Add(TopRightWrap);
	}



	ProceduralMeshRoot->CreateMeshSection_LinearColor(4, Vertices, Triangles, Normals, UVs, Colors, Tangents, true);
	ProceduralMeshRoot->SetMaterial(4, Material);
}
void AVerticalRailActor::GenerateCube(FVector Dimensions)
{
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
	ProceduralMeshRoot->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);


	ProceduralMeshRoot->SetMaterial(0, Material);
}



void AVerticalRailActor::GenerateTopHorizontalRail(FVector Dimensions)
{
	TArray<FVector> Vertices;

	float L = Dimentions.Z/ 2.0f;
	float W = Dimensions.Y/ 4.0f;
	float H = 7;

	//Front Face
	Vertices.Add(FVector(-L, -W+5, -H + (Dimensions.Z / 4))); //0
	Vertices.Add(FVector(-L, W+5, -H + (Dimensions.Z / 4))); //1
	Vertices.Add(FVector(-L, W+5, H + (Dimensions.Z / 4))); //2
	Vertices.Add(FVector(-L, -W+5, H + (Dimensions.Z / 4))); //3

	//Bottom Face
	Vertices.Add(FVector(-L, -W+5, -H + (Dimensions.Z / 4))); //4
	Vertices.Add(FVector(L, -W+5, -H + (Dimensions.Z / 4)));  //5
	Vertices.Add(FVector(L, W+5, -H + (Dimensions.Z / 4)));   //6
	Vertices.Add(FVector(-L, W+5, -H + (Dimensions.Z / 4)));  //7

	//Back Face
	Vertices.Add(FVector(L, -W+5, -H + (Dimensions.Z / 4))); //8
	Vertices.Add(FVector(L, -W+5, H + (Dimensions.Z / 4))); //9
	Vertices.Add(FVector(L, W+5, H + (Dimensions.Z / 4))); //10
	Vertices.Add(FVector(L, W+5, -H + (Dimensions.Z / 4))); //11

	//Top Face
	Vertices.Add(FVector(L, -W+5, H + (Dimensions.Z / 4))); //12
	Vertices.Add(FVector(-L, -W+5, H + (Dimensions.Z / 4))); //13
	Vertices.Add(FVector(-L, W+5, H + (Dimensions.Z / 4)));  //14
	Vertices.Add(FVector(L, W+5, H + (Dimensions.Z / 4)));  //15

	//Left Face
	Vertices.Add(FVector(-L, -W+5, H + (Dimensions.Z / 4))); //16
	Vertices.Add(FVector(L, -W+5, H + (Dimensions.Z / 4))); //17
	Vertices.Add(FVector(L, -W+5, -H + (Dimensions.Z / 4))); //18
	Vertices.Add(FVector(-L, -W+5, -H + (Dimensions.Z / 4))); //19
   
	//Right Face
	Vertices.Add(FVector(-L, W+5, H + (Dimensions.Z / 4))); //20
	Vertices.Add(FVector(-L, W+5, -H + (Dimensions.Z / 4))); //21 
	Vertices.Add(FVector(L, W+5, -H + (Dimensions.Z / 4))); //22
	Vertices.Add(FVector(L, W+5, H + (Dimensions.Z / 4))); //23

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
	ProceduralMeshRoot->CreateMeshSection_LinearColor(2, Vertices, Triangles, Normals, UVs, TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);


	ProceduralMeshRoot->SetMaterial(2, Material);
}

void AVerticalRailActor::GenerateBottomHorizontalRail(FVector Dimensions)
{
	TArray<FVector> Vertices;

	float L = Dimensions.Z / 2.0f;
	float W = Dimensions.Y / 4.0f;
	float H = 7;

	//Front Face
	Vertices.Add(FVector(-L, -W+5, -H + (-Dimensions.Z / 4))); //0
	Vertices.Add(FVector(-L, W+5, -H + (-Dimensions.Z / 4))); //1
	Vertices.Add(FVector(-L, W+5, H + (-Dimensions.Z / 4))); //2
	Vertices.Add(FVector(-L, -W+5, H + (-Dimensions.Z / 4))); //3
	
	//Bottom Face
	Vertices.Add(FVector(-L, -W+5, -H + (-Dimensions.Z / 4))); //4
	Vertices.Add(FVector(L, -W+5, -H + (-Dimensions.Z / 4)));  //5
	Vertices.Add(FVector(L, W+5, -H + (-Dimensions.Z / 4)));   //6
	Vertices.Add(FVector(-L, W+5, -H + (-Dimensions.Z / 4)));  //7

	//Back Face
	Vertices.Add(FVector(L, -W+5, -H + (-Dimensions.Z / 4))); //8
	Vertices.Add(FVector(L, -W+5, H + (-Dimensions.Z / 4))); //9
	Vertices.Add(FVector(L, W+5, H + (-Dimensions.Z / 4))); //10
	Vertices.Add(FVector(L, W+5, -H + (-Dimensions.Z / 4))); //11

	//Top Face
	Vertices.Add(FVector(L, -W+5, H + (-Dimensions.Z / 4))); //12
	Vertices.Add(FVector(-L, -W+5, H + (-Dimensions.Z / 4))); //13
	Vertices.Add(FVector(-L, W+5, H + (-Dimensions.Z / 4)));  //14
	Vertices.Add(FVector(L, W+5, H + (-Dimensions.Z / 4)));  //15

	//Left Face
	Vertices.Add(FVector(-L, -W+5, H + (-Dimensions.Z / 4))); //16
	Vertices.Add(FVector(L, -W+5, H + (-Dimensions.Z / 4))); //17
	Vertices.Add(FVector(L, -W+5, -H + (-Dimensions.Z / 4))); //18
	Vertices.Add(FVector(-L, -W+5, -H + (-Dimensions.Z / 4))); //19

	//Right Face
	Vertices.Add(FVector(-L, W+5, H + (-Dimensions.Z / 4))); //20
	Vertices.Add(FVector(-L, W+5, -H + (-Dimensions.Z / 4))); //21 
	Vertices.Add(FVector(L, W+5, -H + (-Dimensions.Z / 4))); //22
	Vertices.Add(FVector(L, W+5, H + (-Dimensions.Z / 4))); //23

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
	ProceduralMeshRoot->CreateMeshSection_LinearColor(3, Vertices, Triangles, Normals, UVs, TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);


	ProceduralMeshRoot->SetMaterial(3, Material);
}

void AVerticalRailActor::GenerateCurvedCone(const int32& NumSegments, const int32& NumRings, const int32& Radius, const int32& Height) {
	
	TArray<FVector> Vertices;
	TArray<FVector2D> UVs;
	TArray<FVector> Normals;
	TArray<FProcMeshTangent> Tangents;

	TArray<int32> Triangles;

	for (int32 Ring = 0; Ring <= NumRings; ++Ring)
	{
		float RingFraction = (float)Ring / (float)NumRings;
		float Z = Height * (1.0f - RingFraction);
		float RingRadius = Radius * FMath::Sin(RingFraction * PI * 0.5f);

		if (Ring > NumRings * 0.8f) {
			RingRadius *= 0.5f + 0.5f * FMath::Sin((RingFraction - 0.8f) * PI * 2.5f);
		}

		for (int32 Segment = 0; Segment <= NumSegments; ++Segment)
		{
			float SegmentFraction = (float)Segment / (float)NumSegments;
			float Theta = SegmentFraction * 2.0f * PI;
			float X = RingRadius * FMath::Cos(Theta);
			float Y = RingRadius * FMath::Sin(Theta);

			Vertices.Add(FVector(X, Y, Z + 5+(Dimentions.Z/2)));
			Normals.Add(FVector(X, Y, 0).GetSafeNormal());
			UVs.Add(FVector2D(SegmentFraction, RingFraction));
			
			Tangents.Add(FProcMeshTangent(-FMath::Sin(Theta), FMath::Cos(Theta), 0));
		}
	}

	for (int32 Ring = 0; Ring < NumRings; ++Ring)
	{
		for (int32 Segment = 0; Segment < NumSegments; ++Segment)
		{
			int32 A = Ring * (NumSegments + 1) + Segment;
			int32 B = (Ring + 1) * (NumSegments + 1) + Segment;
			int32 C = A + 1;
			int32 D = B + 1;

			Triangles.Add(C);
			Triangles.Add(B);
			Triangles.Add(A);

			Triangles.Add(C);
			Triangles.Add(D);
			Triangles.Add(B);
		}
	}

	ProceduralMeshRoot->CreateMeshSection_LinearColor(5, Vertices, Triangles, Normals, UVs, TArray<FLinearColor>(), Tangents, true);
}


void AVerticalRailActor::GeneratePyramid(FVector Dimensions)
{
	TArray<FVector> Vertices;

	float L = Dimensions.X / 2.0f;
	float W = Dimensions.Y / 2.0f;
	float H = 15;

	Vertices.Add(FVector(-L, -W, 0+(Dimensions.Z/2))); //0
	Vertices.Add(FVector(L, -W, 0+(Dimensions.Z/2))); //1
	Vertices.Add(FVector(L, W, 0+(Dimensions.Z/2))); //2
	Vertices.Add(FVector(-L, W, 0+(Dimensions.Z/2))); //3

	//front
	Vertices.Add(FVector(-L, W, 0+(Dimensions.Z/2)));//4
	Vertices.Add(FVector(0, 0, H+(Dimensions.Z/2))); //5
	Vertices.Add(FVector(-L, -W, 0+(Dimensions.Z/2))); //6

	//left
	Vertices.Add(FVector(-L, -W, 0+(Dimensions.Z/2))); //7
	Vertices.Add(FVector(0, 0, H+(Dimensions.Z/2))); //8
	Vertices.Add(FVector(L, -W, 0+(Dimensions.Z/2)));//9

	//back
	Vertices.Add(FVector(L, -W, 0+(Dimensions.Z/2))); //10
	Vertices.Add(FVector(0, 0, H+(Dimensions.Z/2))); //11
	Vertices.Add(FVector(L, W, 0+(Dimensions.Z/2)));//12

	//right
	Vertices.Add(FVector(L, W, 0+(Dimensions.Z/2))); //13
	Vertices.Add(FVector(0, 0, H+(Dimensions.Z/2))); //14
	Vertices.Add(FVector(-L, W, 0+(Dimensions.Z/2)));//15
	// Triangles

	TArray<int32> Triangles = { 0,1,3, 1,2,3 ,4,5,6,7,8,9,10,11,12,13,14,15 };

	TArray<FVector2D>  UVs = { FVector2D(0,0), FVector2D(0,1), FVector2D(1,1), FVector2D(1,0),
			FVector2D(1,1),FVector2D(0,0.5),FVector2D(0,0),
			FVector2D(1,1),FVector2D(0,0.5),FVector2D(0,0),
			FVector2D(1,1),FVector2D(0,0.5),FVector2D(0,0),
			FVector2D(1,1),FVector2D(0,0.5),FVector2D(0,0),
	};

	ProceduralMeshRoot->CreateMeshSection_LinearColor(1, Vertices, Triangles, TArray<FVector>(), UVs, TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);
	/*ProceduralMeshCap->SetRelativeLocation({ 0,0,Dimensions.Z / 2 });*/

	ProceduralMeshRoot->SetMaterial(1, Material);
}

void AVerticalRailActor::GeneratePyramidOverSphere(FVector Dimensions)
{
	TArray<FVector> Vertices;

	float L = Dimensions.X / 4.0f;
	float W = Dimensions.X / 4.0f;
	float H = 12;

	Vertices.Add(FVector(-L, -W, 0 + ((Dimensions.Z / 2)+7))); //0
	Vertices.Add(FVector(L, -W, 0 + ((Dimensions.Z / 2)+7))); //1
	Vertices.Add(FVector(L, W, 0 + ((Dimensions.Z / 2)+7))); //2
	Vertices.Add(FVector(-L, W, 0 + ((Dimensions.Z / 2)+7))); //3

	//front
	Vertices.Add(FVector(-L, W, 0 + ((Dimensions.Z / 2)+7)));//4
	Vertices.Add(FVector(0, 0, H + ((Dimensions.Z / 2)+7))); //5
	Vertices.Add(FVector(-L, -W, 0 + ((Dimensions.Z / 2)+7))); //6

	//left
	Vertices.Add(FVector(-L, -W, 0 + ((Dimensions.Z / 2)+7))); //7
	Vertices.Add(FVector(0, 0, H + ((Dimensions.Z / 2)+7))); //8
	Vertices.Add(FVector(L, -W, 0 + ((Dimensions.Z / 2)+7)));//9

	//back
	Vertices.Add(FVector(L, -W, 0 + ((Dimensions.Z / 2)+7))); //10
	Vertices.Add(FVector(0, 0, H + ((Dimensions.Z / 2)+7))); //11
	Vertices.Add(FVector(L, W, 0 + ((Dimensions.Z / 2)+7)));//12

	//right
	Vertices.Add(FVector(L, W, 0 + ((Dimensions.Z / 2)+7))); //13
	Vertices.Add(FVector(0, 0, H + ((Dimensions.Z / 2)+7))); //14
	Vertices.Add(FVector(-L, W, 0 + ((Dimensions.Z / 2)+7)));//15
	// Triangles

	TArray<int32> Triangles = { 0,3,1, 1,3,2 ,4,5,6,7,8,9,10,11,12,13,14,15 };

	TArray<FVector2D>  UVs = { FVector2D(0,0), FVector2D(0,1), FVector2D(1,1), FVector2D(1,0),
			FVector2D(1,1),FVector2D(0,0.5),FVector2D(0,0),
			FVector2D(1,1),FVector2D(0,0.5),FVector2D(0,0),
			FVector2D(1,1),FVector2D(0,0.5),FVector2D(0,0),
			FVector2D(1,1),FVector2D(0,0.5),FVector2D(0,0),
	};

	ProceduralMeshRoot->CreateMeshSection_LinearColor(5, Vertices, Triangles, TArray<FVector>(), UVs, TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);
	/*ProceduralMeshCap->SetRelativeLocation({ 0,0,Dimensions.Z / 2 });*/

	ProceduralMeshRoot->SetMaterial(5, Material); 

}


void AVerticalRailActor::GenerateCubePyramid(const FVector& Dimensions)
{
	FVector Dim{ 10,10,10 };
	TArray<FVector> Vertices;

	// Bottom Face
	Vertices.Add(FVector{ -Dim.X / 2, -Dim.Y / 2, (-(Dim.Z / 2))+((Dimensions.Z/2)+7) }); // 0 - - -
	Vertices.Add(FVector{ -Dim.X / 2, Dim.Y / 2, (-(Dim.Z / 2))+((Dimensions.Z/2)+7) }); // 1 - + -
	Vertices.Add(FVector{ Dim.X / 2, -Dim.Y / 2, (-(Dim.Z / 2))+((Dimensions.Z/2)+7) }); // 2 + - -
	Vertices.Add(FVector{ Dim.X / 2, Dim.Y / 2, (-(Dim.Z / 2))+((Dimensions.Z/2)+7) }); // 3 + + -

	// Front Face
	Vertices.Add(FVector{ -Dim.X / 2, -Dim.Y / 2, (-(Dim.Z / 2))+((Dimensions.Z/2)+7) }); // 4 - - - 0
	Vertices.Add(FVector{ -Dim.X / 2, Dim.Y / 2, (-(Dim.Z / 2))+((Dimensions.Z/2)+7) }); // 5 - + - 1
	Vertices.Add(FVector{ -Dim.X / 2, -Dim.Y / 2, (Dim.Z / 2)+((Dimensions.Z/2)+7) }); // 6 - - + 4
	Vertices.Add(FVector{ -Dim.X / 2, Dim.Y / 2, (Dim.Z / 2)+((Dimensions.Z/2)+7) }); // 7 - + + 5

	// Back Face
	Vertices.Add(FVector{ Dim.X / 2, -Dim.Y / 2, (Dim.Z / 2+((Dimensions.Z/2)+7)) }); // 8 + - - 2
	Vertices.Add(FVector{ Dim.X / 2, Dim.Y / 2, (Dim.Z / 2)+((Dimensions.Z/2)+7) }); // 9 + + - 3
	Vertices.Add(FVector{ Dim.X / 2, -Dim.Y / 2, (-(Dim.Z / 2))+((Dimensions.Z/2)+7) });  // 10 + - + 6
	Vertices.Add(FVector{ Dim.X / 2, Dim.Y / 2, (-(Dim.Z / 2))+((Dimensions.Z/2)+7) }); // 11 + + + 7

	// Left Face
	Vertices.Add(FVector{ Dim.X / 2, -Dim.Y / 2, (-(Dim.Z / 2))+((Dimensions.Z/2)+7) }); // 12 + - - 2
	Vertices.Add(FVector{ -Dim.X / 2, -Dim.Y / 2, (-(Dim.Z / 2))+((Dimensions.Z/2)+7) }); // 13 - - - 0
	Vertices.Add(FVector{ Dim.X / 2, -Dim.Y / 2, (Dim.Z / 2)+((Dimensions.Z/2)+7) }); // 14 + - + 6
	Vertices.Add(FVector{ -Dim.X / 2, -Dim.Y / 2, (Dim.Z / 2)+((Dimensions.Z/2)+7) }); // 15 - - + 4+((Dimensions.Z/2)+7)

	// Right Face
	Vertices.Add(FVector{ -Dim.X / 2, Dim.Y / 2, (-(Dim.Z / 2))+((Dimensions.Z/2)+7) }); // 16 + + - 3
	Vertices.Add(FVector{ Dim.X / 2, Dim.Y / 2, (-(Dim.Z / 2))+((Dimensions.Z/2)+7) }); // 17 - + - 1
	Vertices.Add(FVector{ -Dim.X / 2, Dim.Y / 2, (Dim.Z / 2)+((Dimensions.Z/2)+7) }); // 18 + + + 7
	Vertices.Add(FVector{ Dim.X / 2, Dim.Y / 2, (Dim.Z / 2)+((Dimensions.Z/2)+7) }); // 19 - + + 5

	// Triangle Front
	Vertices.Add(FVector{ -Dim.X / 2, -Dim.Y / 2, (Dim.Z / 2) + ((Dimensions.Z / 2) + 7) }); // 20
	Vertices.Add(FVector{ -Dim.X / 2, Dim.Y / 2, (Dim.Z / 2) + ((Dimensions.Z / 2) + 7) }); // 21
	Vertices.Add(FVector{ 0, 0, Dim.Z + ((Dimensions.Z / 2) + 7) }); // 22 

	// Triangle Back
	Vertices.Add(FVector{ Dim.X / 2, Dim.Y / 2, (Dim.Z / 2) + ((Dimensions.Z / 2) + 7) }); // 23
	Vertices.Add(FVector{ Dim.X / 2, -Dim.Y / 2, (Dim.Z / 2) + ((Dimensions.Z / 2) + 7) }); // 24
	Vertices.Add(FVector{ 0, 0, Dim.Z + ((Dimensions.Z / 2) + 7) }); // 25

	// Triangle Left
	Vertices.Add(FVector{ Dim.X / 2, -Dim.Y / 2, (Dim.Z / 2) + ((Dimensions.Z / 2) + 7) }); // 26
	Vertices.Add(FVector{ -Dim.X / 2, -Dim.Y / 2, (Dim.Z / 2) + ((Dimensions.Z / 2) + 7) }); // 27
	Vertices.Add(FVector{ 0, 0, Dim.Z + ((Dimensions.Z / 2) + 7) }); // 28

	// Triangle Right
	Vertices.Add(FVector{ -Dim.X / 2, Dim.Y / 2, (Dim.Z / 2) + ((Dimensions.Z / 2) + 7) }); // 29
	Vertices.Add(FVector{ Dim.X / 2, Dim.Y / 2, (Dim.Z / 2) + ((Dimensions.Z / 2) + 7) }); // 30
	Vertices.Add(FVector{ 0, 0, Dim.Z + ((Dimensions.Z / 2) + 7) }); // 31


	TArray<int32> Triangles = { 2, 3, 0,
		3, 1, 0,
		4, 5, 6 ,
		5, 7, 6 ,
		8, 9, 10,
	9, 11, 10,

	12, 13, 14,
	13, 15, 14,
	16, 17, 18,
	17, 19, 18,

	20, 21, 22,
	23, 24, 25,
	26, 27, 28,

	29, 30, 31 };

	TArray<FVector2D>  UVs;

	TArray<FVector> Normals;
	
	

	for (int32 i = 0; i < 20; i += 4) {
		UVs.Add(FVector2D{ 0.0, 1.0 });
		UVs.Add(FVector2D{ 1.0, 1.0 });
		UVs.Add(FVector2D{ 0.0, 0.0 });
		UVs.Add(FVector2D{ 1.0, 0.0 });
	}

	for (int32 i = 20; i < 32; i += 3) {
		UVs.Add(FVector2D{ 0.0, 0.0 });
		UVs.Add(FVector2D{ 1.0, 0.0 });
		UVs.Add(FVector2D{ 0.0, 1.0 });
	}

	Normals.Add({ 0.0, 0.0, -1.0 });
	Normals.Add({ -1.0, 0.0, 0.0 });
	Normals.Add({ 1.0, 0.0, 0.0 });
	Normals.Add({ 0.0, -1.0, 0.0 });
	Normals.Add({ 0.0, 1.0, 0.0 });

	Normals.Add(FVector::CrossProduct(Vertices[20], Vertices[21]));
	Normals.Add(FVector::CrossProduct(Vertices[23], Vertices[24]));
	Normals.Add(FVector::CrossProduct(Vertices[26], Vertices[27]));
	Normals.Add(FVector::CrossProduct(Vertices[29], Vertices[30]));

	ProceduralMeshRoot->CreateMeshSection_LinearColor(1, Vertices, Triangles, Normals, UVs, TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);


	ProceduralMeshRoot->SetMaterial(1, Material);
}

void AVerticalRailActor::SetMaterial(int Section,UMaterialInterface* MyMaterial)
{
	ProceduralMeshRoot->SetMaterial(Section, MyMaterial);

	ProceduralMeshRoot->SetMaterial(3, MyMaterial);
	ProceduralMeshRoot->SetMaterial(4, MyMaterial);
	ProceduralMeshRoot->SetMaterial(5, MyMaterial);

}


void AVerticalRailActor::GenerateSphere(float Radius, int32 Segments, int32 Rings)
{
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FProcMeshTangent> Tangents;
	TArray<FLinearColor> Colors;

	for (int r = 0; r < Rings; ++r)
	{
		float phi = PI * r / (Rings + 1);
		for (int s = 0; s < Segments; ++s)
		{
			float theta = 2 * PI * s / Segments;
			FVector pos = FVector(Radius * FMath::Sin(phi) * FMath::Cos(theta), Radius * FMath::Sin(phi) * FMath::Sin(theta), Radius * FMath::Cos(phi) + (Dimentions.Z / 2) + Radius + 4);
			Vertices.Add(pos);
			Normals.Add(pos.GetSafeNormal());
			UVs.Add(FVector2D(s / (float)Segments, r / (float)(Rings + 1)));
			FVector tangent = FVector(-FMath::Sin(theta), FMath::Cos(theta), 0).GetSafeNormal();
			Tangents.Add(FProcMeshTangent(tangent, false));
			Colors.Add(FLinearColor(1, 1, 1, 1));

			// We only add extra vertices at the seam on the first ring iteration
			if (s == Segments) {
				// Duplicate the first vertex of each ring at the seam for seamless UVs
				Vertices.Add(Vertices[1 + (r - 1) * (Segments + 1)]);
				Normals.Add(Normals[1 + (r - 1) * (Segments + 1)]);
				UVs.Add(FVector2D(1.f, (float)r / (Rings + 1)));
				Tangents.Add(Tangents[1 + (r - 1) * (Segments + 1)]);
				Colors.Add(FLinearColor(1, 1, 1, 1));
			}
		}
	}

	// Bottom vertex
	Vertices.Add(FVector(0, 0, -Radius));
	Normals.Add(FVector(0, 0, -1));
	UVs.Add(FVector2D(0.5f, 1));
	Tangents.Add(FProcMeshTangent(FVector(1, 0, 0), false));
	Colors.Add(FLinearColor(1, 1, 1, 1));

	// Top Cap
	for (int s = 0; s < Segments; ++s)
	{
		Triangles.Add(0);
		Triangles.Add(1 + s);
		Triangles.Add(1 + (s + 1) % Segments);
	}

	// Body
	for (int r = 0; r < Rings; ++r)
	{
		for (int s = 0; s < Segments; ++s)
		{
			int current = 1 + r * Segments + s;
			int next = current + 1;
			if (s == Segments - 1) next = 1 + r * Segments;  // Wrap around

			int below = current + Segments;
			int belowNext = next + Segments;

			Triangles.Add(current);
			Triangles.Add(next);
			Triangles.Add(belowNext);

			Triangles.Add(current);
			Triangles.Add(belowNext);
			Triangles.Add(below);
		}
	}

	// Bottom Cap
	int bottomIndex = Vertices.Num() - 1;
	int firstOfLastRing = 1 + (Rings - 1) * Segments;
	for (int s = 0; s < Segments; ++s)
	{
		Triangles.Add(bottomIndex);
		Triangles.Add(firstOfLastRing + (s + 1) % Segments);
		Triangles.Add(firstOfLastRing + s);
	}
	// Create the mesh section
	ProceduralMeshRoot->CreateMeshSection_LinearColor(1, Vertices, Triangles, TArray<FVector>(), UVs, TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);
	/*ProceduralMeshCap->SetRelativeLocation({ 0,0,Dimensions.Z / 2 });*/

	ProceduralMeshRoot->SetMaterial(1, Material);

}

void AVerticalRailActor::GenerateCircle(float Radius, int segments) { 
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FProcMeshTangent> Tangents;
	TArray<FLinearColor> Colors;
	for (int i = 0; i < segments; i++) {
		float theta = (PI * 2 * i) / segments;
		Vertices.Add({ Radius * FMath::Cos(theta), Radius * FMath::Sin(theta), Radius });
	}
}

void AVerticalRailActor::GenerateEgg(float Radius, int LatitudeSegments, int LongitudeSegments)
{

	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FProcMeshTangent> Tangents;

	// Generate vertices and triangles
	for (int32 lat = 0; lat <= LatitudeSegments; ++lat)
	{
		float theta = lat * PI / LatitudeSegments;
		float sinTheta = FMath::Sin(theta);
		float cosTheta = FMath::Cos(theta);

		for (int32 lon = 0; lon <= LongitudeSegments; ++lon)
		{
			float phi = lon * 2 * PI / LongitudeSegments;
			float sinPhi = FMath::Sin(phi);
			float cosPhi = FMath::Cos(phi);

			FVector vertex = FVector(cosPhi * sinTheta * Radius, sinPhi * sinTheta * Radius, cosTheta * (Radius+(Radius/1.5)));
			vertex.Z = vertex.Z + (Dimentions.Z / 2) + Radius + 2;

			Vertices.Add(vertex);

			FVector normal = vertex.GetSafeNormal();
			Normals.Add(normal);

			FVector2D uv = FVector2D((float)lon / LongitudeSegments, (float)lat / LatitudeSegments);
			UVs.Add(uv);

			FVector tangentX = FVector(-sinPhi, cosPhi, 0).GetSafeNormal();
			Tangents.Add(FProcMeshTangent(tangentX, false));

			// Generate triangles
			if (lat < LatitudeSegments && lon < LongitudeSegments)
			{
				int32 first = (lat * (LongitudeSegments + 1)) + lon;
				int32 second = first + LongitudeSegments + 1;

				Triangles.Add(first);
				Triangles.Add(second);
				Triangles.Add(first + 1);

				Triangles.Add(second);
				Triangles.Add(second + 1);
				Triangles.Add(first + 1);
			}
		}
	}


	// Create the mesh section
	ProceduralMeshRoot->CreateMeshSection_LinearColor(1, Vertices, Triangles, TArray<FVector>(), UVs, TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);
	/*ProceduralMeshCap->SetRelativeLocation({ 0,0,Dimensions.Z / 2 });*/

	ProceduralMeshRoot->SetMaterial(1, Material);
}

void AVerticalRailActor::CreateSemiDisk(float Radius, float Width, FVector Offset)
{

	//Mesh->ClearAllMeshSections();
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FProcMeshTangent> Tangents;
	TArray<FLinearColor> VertexColors;

	// Calculate points for semicircles
	const int32 NumSegments = 20; // Number of segments in the semicircle
	const float DeltaTheta = 180.0f / (NumSegments - 1);

	Vertices.Add(FVector{ 0,0,0 } + Offset);
	Vertices.Add(FVector{ 0,  Width / 2,0 } + Offset);
	Vertices.Add(FVector{ 0,-Width / 2,0 } + Offset);

	UVs.Add(FVector2D(0.5f, 0.5f)); // Center
	UVs.Add(FVector2D(0.5f, 0.5f)); // Top middle (on front)
	UVs.Add(FVector2D(0.5f, 0.0f)); // Top middle (on back)


	// Top vertices of the semicircles
	for (int32 i = 0; i < NumSegments; i++)
	{
		float AngleRad = FMath::DegreesToRadians(i * DeltaTheta);
		float X = Radius * FMath::Cos(AngleRad);
		float Z = Radius * FMath::Sin(AngleRad);

		// Vertices on Front
		Vertices.Add(FVector(X, Width / 2, Z) + Offset);

		// Vertices on Back
		Vertices.Add(FVector(X, -Width / 2, Z) + Offset);


		// Compute UVs for each vertex
		float U = 0.5f + (X / (2 * Radius)); // Normalize X to [0, 1]
		float V_Front = 0.5f + (Z / (2 * Radius)); // Normalize Z to [0, 1] for front
		float V_Back = 0.5f - (Z / (2 * Radius)); // Normalize Z to [0, 1] for back

		UVs.Add(FVector2D(U, V_Front)); // UV for front vertices
		UVs.Add(FVector2D(U, V_Back));  // UV for back vertices


	}

	// Add triangles for semicircles (front faces)
	{
		int32 CurrentVertexIndex = 3;

		for (int32 i = 0; i < NumSegments - 1; i++)
		{
			Triangles.Add(1);
			Triangles.Add(CurrentVertexIndex);
			Triangles.Add(CurrentVertexIndex + 2);
			CurrentVertexIndex += 2;
		}
	}


	// Add triangles for semicircles (Back faces)

	{
		int32 CurrentVertexIndex = 4;

		for (int32 i = 0; i < NumSegments - 1; i++)
		{
			Triangles.Add(2);
			Triangles.Add(CurrentVertexIndex + 2);
			Triangles.Add(CurrentVertexIndex);
			CurrentVertexIndex += 2;
		}
	}


	Normals.Add({ 0,0,-1 });
	Normals.Add({ 0,1,0 });
	Normals.Add({ 0,-1,0 });


	// Calculate normals and tangents
	for (int32 i = 3; i < Vertices.Num(); i++)
	{
		if (i % 2 != 0)
		{

			Normals.Add({ 0,1,0 });

		}
		else
		{

			Normals.Add({ 0, -1 ,0 });
		}


	}


	{
		int32 CurrentVertexIndex = 3;

		for (int32 i = 0; i < (NumSegments - 1); i++)
		{
			Triangles.Add(CurrentVertexIndex);
			Triangles.Add(CurrentVertexIndex + 1);
			Triangles.Add(CurrentVertexIndex + 2);

			Triangles.Add(CurrentVertexIndex + 2);
			Triangles.Add(CurrentVertexIndex + 1);
			Triangles.Add(CurrentVertexIndex + 3);


			CurrentVertexIndex += 2;
		}
	}


	auto OldTotalVertices = Vertices.Num();

	for (int32 i = 3; i < OldTotalVertices; ++i)
	{
		auto Vertex = Vertices[i];
		Vertices.Add(Vertex);

		auto UV = UVs[i];

		UVs.Add(UV);

	}


	{
		int32 CurrentVertexIndex = OldTotalVertices;

		for (int32 i = 0; i < OldTotalVertices - 3; i++)
		{
			Triangles.Add(CurrentVertexIndex);
			Triangles.Add(CurrentVertexIndex + 1);
			Triangles.Add(CurrentVertexIndex + 2);

			Triangles.Add(CurrentVertexIndex + 2);
			Triangles.Add(CurrentVertexIndex + 1);
			Triangles.Add(CurrentVertexIndex + 3);


			CurrentVertexIndex += 2;
		}
	}


	{
		int32 CurrentVertexIndex = OldTotalVertices;
		// Calculate normals and tangents
		for (int32 i = 0; i < OldTotalVertices - 3; i++)
		{
			Normals.Add(UKismetMathLibrary::GetDirectionUnitVector(Vertices[0], Vertices[CurrentVertexIndex]));
			CurrentVertexIndex++;
		}
	}



	// Create mesh section
	ProceduralMeshRoot->CreateMeshSection_LinearColor(1, Vertices, Triangles, Normals, UVs, TArray<FLinearColor>(), Tangents, true);
	ProceduralMeshRoot->SetMaterial(1, Material);
}
void AVerticalRailActor::BeginPlay()
{
}

void AVerticalRailActor::Tick(float DeltaTime)
{
}


