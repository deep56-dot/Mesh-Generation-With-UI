// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncTaskHandler.h"
#include "MeshGenerator.h"

FAsyncTaskHandler::FAsyncTaskHandler(AMeshGenerator* InMeshGenerator)
{
	MeshGenerator = InMeshGenerator;
}

void FAsyncTaskHandler::DoWork()
{
	if (MeshGenerator)
	{
		if (UMeshDataAsset* DataAsset = MeshGenerator->DataAsset)
		{
			TArray<FMeshDataStruct> MeshDataStruct = DataAsset->MeshDataArr;


			for (int jIndex = 0; jIndex < MeshDataStruct.Num(); jIndex++)
			{
				//int32 RandomMeshIndex = FMath::RandRange(0, StaticMeshes.Num() - 1);
				UStaticMesh* CurrentMesh = MeshDataStruct[jIndex].MeshType;


				for (int iIndex = 0; iIndex < MeshGenerator->NumberOfInstances; iIndex++)
				{
					float Rotation= FMath::RandRange(MeshDataStruct[jIndex].MinRotation, MeshDataStruct[jIndex].MaxRotation);
					float Scale= FMath::RandRange(MeshDataStruct[jIndex].MinScale, MeshDataStruct[jIndex].MaxScale);
					FVector Position;
					if (MeshGenerator->Type == "Box") {
					//Box
						GEngine->AddOnScreenDebugMessage(
							-1,
							2.0f,
							FColor::Cyan,
							FString::Printf(TEXT("Box Generate")));
					FVector BoundingExtent = MeshGenerator->Scale*100;
					FVector Origin = MeshGenerator->Location;
					FBox BoundingBox(Origin - BoundingExtent, Origin + BoundingExtent);

			         Position = FMath::RandPointInBox(BoundingBox);
					}
					else {
						GEngine->AddOnScreenDebugMessage(
							-1,
							2.0f,
							FColor::Cyan,
							FString::Printf(TEXT("Sphere Generate")));
					//Sphere
					float Radius = MeshGenerator->Scale.Z*100;
					FVector Origin = MeshGenerator->Location;
					Position = FMath::VRand() * FMath::FRandRange(0.0f, Radius) + Origin;
					}


					GEngine->AddOnScreenDebugMessage(
						-1,
						2.0f,
						FColor::Cyan,
						FString::Printf(TEXT("Random Position: %s"), *Position.ToString()));
					//InstanceTransforms.Add(FTransform(Position));

					TArray<FTransform> InstanceTransforms;
					FTransform transform;
					transform.SetLocation(Position);
					transform.SetScale3D(FVector(Scale));
					FRotator Rotation_(Rotation, Rotation, Rotation);
					FQuat QuatRotation(Rotation_);

					transform.SetRotation(QuatRotation);					
					InstanceTransforms.Add(transform);

					MeshGenerator->AddInstances(CurrentMesh, InstanceTransforms);
				}


			}

		}

		AsyncTask(ENamedThreads::GameThread, [this]()
			{
				MeshGenerator->FinishScatter();
			});
	}
}
