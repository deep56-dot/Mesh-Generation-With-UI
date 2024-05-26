// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <ProceduralMeshComponent.h>
#include "VerticalRailActor.generated.h"

UENUM()
enum class ETypeOfRail {
	Sphere,
	Pyramid,
	Egg,
	Windsor,
	Extra
};
UCLASS()
class ASSIGNMENT_5_6_API AVerticalRailActor : public AActor
{
	GENERATED_BODY()
	

public:
	// Sets default values for this actor's properties
	AVerticalRailActor();

	virtual void OnConstruction(const FTransform& Transform) override;

	void GenerateBellShape(FVector Location, float BaseRadius, float Height1, float RimRadius, float CurvatureFactor, int NumSlices, int NumStacks);

	UFUNCTION(BlueprintCallable)
	void GenerateCube(FVector Dimensions);



	void GenerateTopHorizontalRail(FVector Dimensions);

	void GenerateBottomHorizontalRail(FVector Dimensions);

	void GenerateCurvedCone(const int32& NumSegments, const int32& NumRings, const int32& Radius, const int32& Height);

	void GeneratePyramid(FVector Dimensions);



	void GenerateCubePyramid(const FVector& Dimensions);

protected:
	void GenerateSphere(float Radius, int LatitudeSegments, int LongitudeSegments);
	void GenerateEgg(float Radius, int LatitudeSegments, int LongitudeSegments);
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProcMesh")
	FVector Dimentions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProcMesh")
	UProceduralMeshComponent* ProceduralMeshRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProcMesh")
	UProceduralMeshComponent* ProceduralMeshCap;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProcMesh")
	TArray<FVector> Normals;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProcMesh")
	TArray<FVector2D>  UVs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProcMesh")
	TArray<FProcMeshTangent>  Tangents;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProcMesh")
	UMaterial* Material;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProcMesh")
	ETypeOfRail RailingType;
};
