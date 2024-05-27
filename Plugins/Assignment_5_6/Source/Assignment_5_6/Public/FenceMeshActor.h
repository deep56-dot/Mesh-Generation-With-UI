// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VerticalRailActor.h"
#include "GameFramework/Actor.h"
#include <Components/SplineComponent.h>
#include "FenceMeshActor.generated.h"

USTRUCT(BlueprintType)
struct FFenceProperties {
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	float length;
	UPROPERTY(EditAnywhere)
	float width;
	UPROPERTY(EditAnywhere)
	float height;
	UPROPERTY(EditAnywhere)
	float spacing;

	FFenceProperties()
		: length(10.0f)
		, width(10.0f)
		, height(100.0f)
		, spacing(50.0f){}

	void ClampValues()
	{
		spacing = FMath::Clamp(spacing,20, height);
	}
};
UCLASS()
class ASSIGNMENT_5_6_API AFenceMeshActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFenceMeshActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void ClearStaticFenceComponents();

	void AddStaticFenceComponents();

	void ReplaceStaticMeshesWithProceduralMesh();
	

	virtual void OnConstruction(const FTransform& Transform) override;

	void OnConstructionProcedural();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProcMesh")
	FFenceProperties FenceProperties;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProcMesh")
	TSubclassOf<AVerticalRailActor>  VerticalRailActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProcMesh")
	UStaticMesh*  StaticVerticalRailActor;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProcMesh")
	int SplinePoints;



	UPROPERTY()
	USceneComponent* SceneComponent;

	UPROPERTY()
	TArray<UStaticMeshComponent*> StaticVerticalRailArr;

	UPROPERTY()
	TArray<AVerticalRailActor*> VerticalRailArr;


	UPROPERTY()
	USplineComponent* SplineComponent;
};
