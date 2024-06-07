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
				UStaticMesh* CurrentMesh = MeshDataStruct[jIndex].MeshType;


				for (int iIndex = 0; iIndex < MeshGenerator->NumberOfInstances; iIndex++)
				{
					float Rotation= FMath::RandRange(MeshDataStruct[jIndex].MinRotation, MeshDataStruct[jIndex].MaxRotation);
					float Scale= FMath::RandRange(MeshDataStruct[jIndex].MinScale, MeshDataStruct[jIndex].MaxScale);
					FVector Position;
					if (MeshGenerator->Type == "Box") {
					
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


					

					TArray<FTransform> InstanceTransforms;
					FTransform transform;
					FRotator Rotation_(Rotation, Rotation, Rotation);
					FQuat QuatRotation(Rotation_);
					transform.SetLocation(Position);
					transform.SetScale3D(FVector(Scale));

					transform.SetRotation(QuatRotation);					
					InstanceTransforms.Add(transform);

					MeshGenerator->AddInstances(CurrentMesh, InstanceTransforms);
					FPlatformProcess::Sleep(0.001f);

					float Progress = (float)(((jIndex) * MeshGenerator->NumberOfInstances) + (iIndex + 1)) / (float)(MeshGenerator->NumberOfInstances * DataAsset->MeshDataArr.Num());

				AsyncTask(ENamedThreads::GameThread, [this ,Progress ]()
					{
						//MeshGenerator->FinishScatter();
						MeshGenerator->UpdateProgressBar(Progress);

					});
				}

			}

		}

		
	}
}
