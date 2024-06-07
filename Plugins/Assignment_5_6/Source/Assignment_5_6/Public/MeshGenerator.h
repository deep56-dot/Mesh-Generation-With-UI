// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "AsyncTaskHandler.h"
#include <MeshDataAsset.cpp>
#include <Components/BoxComponent.h>
#include "MeshGenerator.generated.h"

UCLASS()
class ASSIGNMENT_5_6_API AMeshGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMeshGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AddInstances(UStaticMesh* StaticMesh, const TArray<FTransform>& Transforms);


	UPROPERTY(EditAnywhere, Category = "Scatter")
	UMeshDataAsset* DataAsset;

	UPROPERTY(EditAnywhere, Category = "Scatter")
	FVector Location;

	UPROPERTY(EditAnywhere, Category = "Scatter")
	FVector Scale;
	UPROPERTY(EditAnywhere, Category = "Scatter")
	float Radius;

	UPROPERTY(EditAnywhere, Category = "Scatter")
	int32 NumberOfInstances = 100;

	UPROPERTY(EditAnywhere, Category = "Scatter")
	FString Type;

	UPROPERTY(EditAnywhere, Category = "Scatter")
	UMaterialInstance* Material;

	UFUNCTION(BlueprintCallable)
	void ScatterObjects(int32 N, FVector Scale_, FVector Location_,FString Type_);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateProgressBar(float percent);


	UPROPERTY()
	int32 count = 0;
private:

	FAsyncTask<FAsyncTaskHandler>* AsyncScatterTask;

	UPROPERTY()
	TMap<UStaticMesh*, UHierarchicalInstancedStaticMeshComponent*> HISMComponents;


};
