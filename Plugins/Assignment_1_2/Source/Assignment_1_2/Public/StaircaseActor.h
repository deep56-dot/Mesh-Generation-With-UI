// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StaircaseActor.generated.h"
UENUM(BlueprintType)
enum class StairType {
	Box UMETA(DisplayName = "Box Stairs"),
	Open UMETA(DisplayName = "Open Stairs"),
	Close UMETA(DisplayName = "Close Stairs")

};

USTRUCT(BlueprintType)
struct FMesh {

	GENERATED_BODY()

public:
	UPROPERTY()
	UStaticMeshComponent* Stair;
	UPROPERTY()
	UStaticMeshComponent* Left;
	UPROPERTY()
	UStaticMeshComponent* Right;

	FMesh() {
		Stair = nullptr;
		Left = nullptr;
		Right = nullptr;
	}

	static void DestroyMeshCompomnent(TArray<FMesh>);

};
UCLASS()
class ASSIGNMENT_1_2_API AStaircaseActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AStaircaseActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Edit Staircase")
	int NoOfSteps;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Edit Staircase")
	bool Railing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Edit Staircase")
	StairType TypeOfStair;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Edit Staircase", meta = (EditCondition = "TypeOfStair==StairType::Open", ClampMin = "1.2", UMin = "1.2", ClampMax = "2.0", UMax = "2.0"))
	float spacing;




	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Edit Staircase")
	USceneComponent* RootScene;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Edit Staircase")
	UStaticMesh* StaircaseMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Edit Staircase")
	UStaticMesh* RailingMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Edit Staircase")
	UMaterial* RailingMaterial;



	UPROPERTY()
	TArray<FMesh> StructArr;

	UPROPERTY()
	TArray<UStaticMeshComponent*> StairArr;

	UPROPERTY()
	TArray<UStaticMeshComponent*> LeftRailArr;

	UPROPERTY()
	TArray<UStaticMeshComponent*> RightRailArr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Edit Staircase")
	float length;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Edit Staircase")
	float width;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Edit Staircase")
	float depth;
};
