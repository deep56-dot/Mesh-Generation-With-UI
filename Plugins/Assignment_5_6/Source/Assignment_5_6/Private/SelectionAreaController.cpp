// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectionAreaController.h"




ASelectionAreaController::ASelectionAreaController()
{
    bShowMouseCursor = true;
    DefaultMouseCursor = EMouseCursor::Crosshairs;
    bMove = true;
}

void ASelectionAreaController::BeginPlay()
{
    Super::BeginPlay();
    SelectionActor = GetWorld()->SpawnActor<ASelectionArea>(ASelectionArea::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
    
}

void ASelectionAreaController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    //FVector WorldLocation, WorldDirection;
    //DeprojectMousePositionToWorld(WorldLocation, WorldDirection);

    //FVector Location = WorldLocation + WorldDirection * 1000.0f; // Adjust the distance as needed
    //if(SelectionActor && bMove){
    //    SelectionActor->SetActorLocation(Location);
    //}

    FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true, SelectionActor);
    FHitResult HitResult;
    FVector CursorWorldLocation;
    FVector CursorWorldDirection;
    DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);

    if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams)) {
        FVector Location = HitResult.Location;

        if(SelectionActor && bMove){
           Location.Z+=( SelectionActor->GetActorRelativeScale3D().Z*100);
       SelectionActor->SetActorLocation(Location);
   }
    }
}






