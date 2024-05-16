// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveArchController.h"

void AInteractiveArchController::SetupInputComponent()
{
    Super::SetupInputComponent();

    NewObject<APlayerController>(this, AInteractiveArchController::StaticClass());

    UInputMappingContext* MyMapping = NewObject<UInputMappingContext>(this);
    UInputAction* ClickAction = NewObject<UInputAction>(this);
    ClickAction->ValueType = EInputActionValueType::Boolean;
    MyMapping->MapKey(ClickAction, EKeys::LeftMouseButton);

    UInputAction* ToggleAction = NewObject<UInputAction>(this);
    ToggleAction->ValueType = EInputActionValueType::Boolean;
    MyMapping->MapKey(ToggleAction, EKeys::Tab);

    UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent);

    if (EIC) {
        EIC->BindAction(ClickAction, ETriggerEvent::Completed, this, &AInteractiveArchController::Click);
        EIC->BindAction(ToggleAction, ETriggerEvent::Completed, this, &AInteractiveArchController::HideVisiblity);
    }

    ULocalPlayer* LocalPlayer = GetLocalPlayer();

    UEnhancedInputLocalPlayerSubsystem* SubSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

    SubSystem->AddMappingContext(MyMapping, 0);
}

AInteractiveArchController::AInteractiveArchController()
{
    bShowMouseCursor = true;
    DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AInteractiveArchController::BeginPlay()
{
    Super::BeginPlay();
    SelectionWidgetInstance = CreateWidget<USelectionBox>(this, SelectionWidget);

    if (SelectionWidgetInstance)
        SelectionWidgetInstance->AddToViewport();

    SelectionWidgetInstance->MeshSelectionScrollBoxWidget->OnMyController.BindUObject(this, &AInteractiveArchController::SpawnActor);
    SelectionWidgetInstance->MaterialSelectionScrollBoxWidget->MaterialController.BindUObject(this, &AInteractiveArchController::SetMaterial);
    SelectionWidgetInstance->TextureSelectionScrollBoxWidget->TextureController.BindUObject(this, &AInteractiveArchController::SetTexture);
}


void AInteractiveArchController::Click()
{
    FHitResult HitResult;
    FVector MouseLocation, MouseDirection;

    if (DeprojectMousePositionToWorld(MouseLocation, MouseDirection)) {

        FVector EndLocation = MouseLocation + MouseDirection * 7000;
        FCollisionQueryParams Params;
        Params.AddIgnoredActor(this);
        GetWorld()->LineTraceSingleByChannel(HitResult, MouseLocation, EndLocation, ECC_Visibility);

        if (HitResult.bBlockingHit) {
            AArchMeshActor* ArchActor = Cast<AArchMeshActor>(HitResult.GetActor());
            HitLocation = HitResult.Location;
            bMyActor = true;
            if (ArchActor) {
                bMyActor = false;
                StaticMeshActor = ArchActor;
                HitLocation = StaticMeshActor->GetActorLocation();
                GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, "Actor");
                SelectionWidgetInstance->MeshSelectionScrollBoxWidget->SetVisibility(ESlateVisibility::Visible);
                SelectionWidgetInstance->MaterialSelectionScrollBoxWidget->SetVisibility(ESlateVisibility::Visible);
                SelectionWidgetInstance->TextureSelectionScrollBoxWidget->SetVisibility(ESlateVisibility::Visible);
                DynamicMaterial = UMaterialInstanceDynamic::Create(StaticMeshActor->GetStaticMeshComponent()->GetMaterial(0), this);

            }
            else {
                GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, "Floor");
                SelectionWidgetInstance->MeshSelectionScrollBoxWidget->SetVisibility(ESlateVisibility::Visible);

            }
            GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, *FString::Printf(TEXT("%s"), *HitResult.Location.ToString()));
        }
    }
}

void AInteractiveArchController::HideVisiblity() {
    SelectionWidgetInstance->MeshSelectionScrollBoxWidget->SetVisibility(ESlateVisibility::Hidden);
    SelectionWidgetInstance->MaterialSelectionScrollBoxWidget->SetVisibility(ESlateVisibility::Hidden);
    SelectionWidgetInstance->TextureSelectionScrollBoxWidget->SetVisibility(ESlateVisibility::Hidden);
}

void AInteractiveArchController::SpawnActor(const FMeshData& MeshData)
{
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    FRotator Rotation = FRotator::ZeroRotator;
    SelectionWidgetInstance->MaterialSelectionScrollBoxWidget->SetVisibility(ESlateVisibility::Visible);
    SelectionWidgetInstance->TextureSelectionScrollBoxWidget->SetVisibility(ESlateVisibility::Visible);
    if (!bMyActor) {
        StaticMeshActor->Destroy();

        StaticMeshActor = GetWorld()->SpawnActor<AArchMeshActor>(AArchMeshActor::StaticClass(), HitLocation, Rotation, SpawnParams);
    }
    else {
        StaticMeshActor = GetWorld()->SpawnActor<AArchMeshActor>(AArchMeshActor::StaticClass(), HitLocation, Rotation, SpawnParams);
    }
    if (StaticMeshActor) {
        GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, "Spawn");

        StaticMeshActor->SetMobility(EComponentMobility::Movable);
        StaticMeshActor->GetStaticMeshComponent()->SetStaticMesh(MeshData.Mesh);
        DynamicMaterial = UMaterialInstanceDynamic::Create(StaticMeshActor->GetStaticMeshComponent()->GetMaterial(0), this);

    }
    bMyActor = false;

}

void AInteractiveArchController::SetMaterial(const FMaterialData& MaterialData)
{
    if (StaticMeshActor) {
        StaticMeshActor->GetStaticMeshComponent()->SetMaterial(0, MaterialData.Material);
        DynamicMaterial = UMaterialInstanceDynamic::Create(StaticMeshActor->GetStaticMeshComponent()->GetMaterial(0), this);
    }
}

void AInteractiveArchController::SetTexture(const FTextureData& TextureData)
{
    if (DynamicMaterial) {

        GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, "2");
        DynamicMaterial->SetTextureParameterValue("ParamTexture", TextureData.Texture); // Set texture parameter in material
        StaticMeshActor->GetStaticMeshComponent()->SetMaterial(0, DynamicMaterial);
    }
}