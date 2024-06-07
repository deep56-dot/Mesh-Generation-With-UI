// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshGenerator.h"
#include "Misc/ScopedSlowTask.h"

// Sets default values
AMeshGenerator::AMeshGenerator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoundingVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("BoundingVolume"));
	//BoundingVolume->SetBoxExtent(FVector(1000));
	RootComponent = BoundingVolume;

	DataAsset = LoadObject<UMeshDataAsset>(this, TEXT("/Script/Assignment_5_6.MeshDataAsset'/Assignment_5_6/MeshDataAsset.MeshDataAsset'"));
}

// Called when the game starts or when spawned
void AMeshGenerator::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMeshGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DataAsset)
	{
		UDataAsset* dAsset = DataAsset;

	}
}

void AMeshGenerator::ScatterObjects(int N, FVector Scale_, FVector Location_,FString Type_)
{
	NumberOfInstances = N;
	Location = Location_;
	Scale = Scale_;
	Type = Type_;
	for (auto& Pair : HISMComponents)
	{
		if (UHierarchicalInstancedStaticMeshComponent* HISM = Pair.Value)
		{
			HISM->ClearInstances();
		}
	}

	if (DataAsset)
	{
		SlowTask = new FScopedSlowTask(NumberOfInstances * DataAsset->MeshDataArr.Num(), FText::FromString("Scattering Objects..."));
		SlowTask->MakeDialog(true);
	}

	if (AsyncScatterTask && !AsyncScatterTask->IsDone())
	{
		AsyncScatterTask->EnsureCompletion();
		delete AsyncScatterTask;
		AsyncScatterTask = nullptr;
	}

	AsyncScatterTask = new FAsyncTask<FAsyncTaskHandler>(this);
	AsyncScatterTask->StartBackgroundTask();
}

void AMeshGenerator::AddInstances(UStaticMesh* StaticMesh, const TArray<FTransform>& Transforms)
{
	AsyncTask(ENamedThreads::GameThread, [this, StaticMesh, Transforms]()
		{
	UHierarchicalInstancedStaticMeshComponent** HISMCPtr = HISMComponents.Find(StaticMesh);
	if (HISMCPtr && *HISMCPtr && (*HISMCPtr)->IsValidLowLevel())
	{
		
				(*HISMCPtr)->AddInstances(Transforms, false);
				if (SlowTask)
				{
					SlowTask->EnterProgressFrame(Transforms.Num(), FText::FromString("Scattering Mesh : " + StaticMesh->GetName()));
				}
			
	}
	else
	{
		UHierarchicalInstancedStaticMeshComponent* NewHISMC = NewObject<UHierarchicalInstancedStaticMeshComponent>(this);
		NewHISMC->SetStaticMesh(StaticMesh);

		HISMComponents.Add(StaticMesh, NewHISMC);
				NewHISMC->RegisterComponentWithWorld(GetWorld());
				NewHISMC->AddInstances(Transforms, false);
				NewHISMC->SetMaterial(0,Material);

				if (SlowTask)
				{
					SlowTask->EnterProgressFrame(Transforms.Num(), FText::FromString("Scattering Mesh : " + StaticMesh->GetName()));
				}
			


	}

});
}

void AMeshGenerator::FinishScatter()
{
	if (SlowTask)
	{
		SlowTask->Destroy();
	}

}
