// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <ProceduralMeshComponent.h>
#include "SelectionArea.generated.h"

UCLASS()
class ASSIGNMENT_5_6_API ASelectionArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASelectionArea();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION(Blueprintcallable)
	void ToggleShape();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void GenerateCube(FVector Dimensions);

	void GenerateSphere(float Radius, int32 Segments, int32 Rings);

	UPROPERTY()
	UProceduralMeshComponent* ProceduralMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseSphere;

	UFUNCTION(BlueprintCallable)
	void ChangeDimentions(FVector Dimensions);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* TranslucentMaterial = LoadObject<UMaterialInterface>(this, TEXT("/Script/Engine.Material'/Engine/ArtTools/RenderToTexture/Materials/M_RadiusDebugTranslucent.M_RadiusDebugTransLucent'"));
};
