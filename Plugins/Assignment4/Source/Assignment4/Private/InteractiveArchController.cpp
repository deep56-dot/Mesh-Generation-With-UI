// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveArchController.h"

void AInteractiveArchController::KeyMap(UInputMappingContext* InputMappingContext, UInputAction* InputAction, FKey Key,
    bool bNegate = false,
    bool bSwizzle = false, EInputAxisSwizzle SwizzleOrder = EInputAxisSwizzle::YXZ)

{
    FEnhancedActionKeyMapping& Mapping = InputMappingContext->MapKey(InputAction, Key);
    UObject* Outer = InputMappingContext->GetOuter();

    if (bNegate)
    {
        UInputModifierNegate* Negate = NewObject<UInputModifierNegate>(Outer);
        Mapping.Modifiers.Add(Negate);
    }

    if (bSwizzle)
    {
        UInputModifierSwizzleAxis* Swizzle = NewObject<UInputModifierSwizzleAxis>(Outer);
        Swizzle->Order = SwizzleOrder;
        Mapping.Modifiers.Add(Swizzle);
    }
}
void AInteractiveArchController::SetupInputComponent()
{
    Super::SetupInputComponent();

    NewObject<APlayerController>(this, AInteractiveArchController::StaticClass());

     MyMapping = NewObject<UInputMappingContext>(this);
     CameraMapping = NewObject<UInputMappingContext>(this);
    UInputAction* ClickAction = NewObject<UInputAction>(this);
    ClickAction->ValueType = EInputActionValueType::Boolean;
    MyMapping->MapKey(ClickAction, EKeys::LeftMouseButton);

    UInputAction* ToggleAction = NewObject<UInputAction>(this);
    ToggleAction->ValueType = EInputActionValueType::Boolean;
    MyMapping->MapKey(ToggleAction, EKeys::Tab);

    UInputAction* ChangeViewAction = NewObject<UInputAction>(this);
    ChangeViewAction->ValueType = EInputActionValueType::Boolean;
    CameraMapping->MapKey(ChangeViewAction, EKeys::P);


    //Wall Spline
    SplineMappingContext = NewObject<UInputMappingContext>(this); 

    LeftClickAction = NewObject<UInputAction>(this); 
    LeftClickAction->ValueType = EInputActionValueType::Boolean; 
    KeyMap(SplineMappingContext, LeftClickAction, EKeys::LeftMouseButton); 
      
    RightClickAction = NewObject<UInputAction>(this); 
    RightClickAction->ValueType = EInputActionValueType::Boolean; 
    KeyMap(SplineMappingContext, RightClickAction, EKeys::RightMouseButton); 
      
    UndoAction = NewObject<UInputAction>(this);
    UndoAction->ValueType = EInputActionValueType::Boolean;
    KeyMap(SplineMappingContext, UndoAction, EKeys::Z);

    LeftArrow = NewObject<UInputAction>(this);
    LeftArrow->ValueType = EInputActionValueType::Boolean;
    KeyMap(SplineMappingContext, LeftArrow, EKeys::Left);

    RightArrow = NewObject<UInputAction>(this);
    RightArrow->ValueType = EInputActionValueType::Boolean;
    KeyMap(SplineMappingContext, RightArrow, EKeys::Right);

    UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent);

    if (EIC) {
        EIC->BindAction(ClickAction, ETriggerEvent::Completed, this, &AInteractiveArchController::Click);
        EIC->BindAction(ToggleAction, ETriggerEvent::Completed, this, &AInteractiveArchController::HideVisiblity);
        EIC->BindAction(ChangeViewAction, ETriggerEvent::Completed, this, &AInteractiveArchController::ChangeView);



        EIC->BindAction(LeftClickAction, ETriggerEvent::Completed, this, &AInteractiveArchController::OnLeftClick);
        EIC->BindAction(RightClickAction, ETriggerEvent::Completed, this, &AInteractiveArchController::OnRightClick);
        EIC->BindAction(UndoAction, ETriggerEvent::Completed, this, &AInteractiveArchController::Undo);
        EIC->BindAction(LeftArrow, ETriggerEvent::Completed, this, &AInteractiveArchController::OnLeft);
        EIC->BindAction(RightArrow, ETriggerEvent::Completed, this, &AInteractiveArchController::OnRight);
    }

     LocalPlayer = GetLocalPlayer();

    SubSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
 
    SubSystem->AddMappingContext(MyMapping, 0);
    SubSystem->AddMappingContext(CameraMapping, 0);

}

AInteractiveArchController::AInteractiveArchController()
{
    WallConstructionDelegate.BindUFunction(this, "ChangeText");

    bShowMouseCursor = true;
    DefaultMouseCursor = EMouseCursor::Crosshairs;

    CurrentPawn = Cast<AIsometricCameraPawn>(APawn::StaticClass());
    if (CurrentPawn) {
        Possess(CurrentPawn);
        //GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Black, TEXT("not sapwefns"));
    }
    cnt = 1;
}

void AInteractiveArchController::BeginPlay()
{
    Super::BeginPlay();

    AWallSpline* WallObj = NewObject<AWallSpline>(this);
    WallSplineArr.Add(WallObj);
    CurrWall = WallSplineArr.Num() - 1;

    SelectionWidgetInstance = CreateWidget<USelectionBox>(this, SelectionWidget);
    MyWidgetInstance = CreateWidget<UMyWidget>(this, MyWidget);

    if (SelectionWidgetInstance)
    {
        SelectionWidgetInstance->AddToViewport();

        SelectionWidgetInstance->MeshSelectionScrollBoxWidget->SetVisibility(ESlateVisibility::Hidden);
        SelectionWidgetInstance->MaterialSelectionScrollBoxWidget->SetVisibility(ESlateVisibility::Hidden);
        SelectionWidgetInstance->TextureSelectionScrollBoxWidget->SetVisibility(ESlateVisibility::Hidden);
    }

    SelectionWidgetInstance->MeshSelectionScrollBoxWidget->OnMyController.BindUObject(this, &AInteractiveArchController::SpawnActor);
    SelectionWidgetInstance->MaterialSelectionScrollBoxWidget->MaterialController.BindUObject(this, &AInteractiveArchController::SetMaterial);
    SelectionWidgetInstance->TextureSelectionScrollBoxWidget->TextureController.BindUObject(this, &AInteractiveArchController::SetTexture);
  

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    //GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, "in");

    FVector SpawnLocation_ = FVector(50, 50, 100);
    FRotator SpawnRotation = FRotator(0, 0, 0);
    bAssignment3 = false;
   SpawnedPawn = GetWorld()->SpawnActor<AOrthographicCameraPawn>(AOrthographicCameraPawn::StaticClass(), SpawnLocation_, FRotator::ZeroRotator, SpawnParams);
    if (SpawnedPawn)
    {
        //GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, "Ortho");
        Possess(SpawnedPawn);
        if (bAssignment3)
            SubSystem->AddMappingContext(SplineMappingContext, 0);
        else
            SubSystem->AddMappingContext(MyMapping, 0);
            
        SubSystem->AddMappingContext(CameraMapping, 0);

    }
    num = 2;
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
                //GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, "Actor");
                SelectionWidgetInstance->MeshSelectionScrollBoxWidget->SetVisibility(ESlateVisibility::Visible);
                SelectionWidgetInstance->MaterialSelectionScrollBoxWidget->SetVisibility(ESlateVisibility::Visible);
                SelectionWidgetInstance->TextureSelectionScrollBoxWidget->SetVisibility(ESlateVisibility::Visible);
                DynamicMaterial = UMaterialInstanceDynamic::Create(StaticMeshActor->GetStaticMeshComponent()->GetMaterial(0), this);
                ViewHitLocation = HitLocation;
                num--;
                ChangeView();
            }
            else {
                //GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, "Floor");
                SelectionWidgetInstance->MeshSelectionScrollBoxWidget->SetVisibility(ESlateVisibility::Visible);

            }
            //GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, *FString::Printf(TEXT("%s"), *HitResult.Location.ToString()));
        }
    }
}

void AInteractiveArchController::HideVisiblity() {
    SelectionWidgetInstance->MeshSelectionScrollBoxWidget->SetVisibility(ESlateVisibility::Hidden);
    SelectionWidgetInstance->MaterialSelectionScrollBoxWidget->SetVisibility(ESlateVisibility::Hidden);
    SelectionWidgetInstance->TextureSelectionScrollBoxWidget->SetVisibility(ESlateVisibility::Hidden);
}

void AInteractiveArchController::ChangeView() {
    num = ((num + 1) % 3);
    //GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("%d"), num));

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    FVector SpawnLocation_ = ViewHitLocation+FVector(0,0,50);
    FRotator SpawnRotation = FRotator(0, 0, 0);
    if (num == 1) {
        SpawnedPawn->Destroy();
        SpawnedPawn= GetWorld()->SpawnActor<APerspectiveCameraPawn>(APerspectiveCameraPawn::StaticClass(), SpawnLocation_ , FRotator::ZeroRotator, SpawnParams);
        if (SpawnedPawn)
        {
   
            Possess(SpawnedPawn);
            if (bAssignment3)
                SubSystem->AddMappingContext(SplineMappingContext, 0);
            else
                SubSystem->AddMappingContext(MyMapping, 0);

            SubSystem->AddMappingContext(CameraMapping, 0);

        }
    }
    else if(num==2)
    {
        SpawnedPawn->Destroy();

        SpawnedPawn = GetWorld()->SpawnActor<AOrthographicCameraPawn>(AOrthographicCameraPawn::StaticClass(), SpawnLocation_, FRotator::ZeroRotator, SpawnParams);
        if (SpawnedPawn)
        {
         
            Possess(SpawnedPawn);
            if (bAssignment3)
                SubSystem->AddMappingContext(SplineMappingContext, 0);
            else
                SubSystem->AddMappingContext(MyMapping, 0);
            SubSystem->AddMappingContext(CameraMapping, 0);

        }
    }
    else
    {
        SpawnedPawn->Destroy();

        SpawnedPawn = GetWorld()->SpawnActor<AIsometricCameraPawn>(AIsometricCameraPawn::StaticClass(), SpawnLocation_, FRotator::ZeroRotator, SpawnParams);
        if (SpawnedPawn)
        {
           
            Possess(SpawnedPawn);
            if (bAssignment3)
                SubSystem->AddMappingContext(SplineMappingContext, 0);
            else
                SubSystem->AddMappingContext(MyMapping, 0);
            SubSystem->AddMappingContext(CameraMapping, 0);

            
        }
    }

}

void AInteractiveArchController::Switch() {
    if (cnt % 2 == 1) {
        bAssignment3 = true;
        SelectionWidgetInstance->RemoveFromViewport();
        MyWidgetInstance->AddToViewport();
        //GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, "jausdkhs");
        SubSystem->ClearAllMappings();
        SubSystem->AddMappingContext(SplineMappingContext, 0);
        SubSystem->AddMappingContext(CameraMapping, 0);
    }
    else {
        bAssignment3 = false;

        MyWidgetInstance->RemoveFromViewport();
        SelectionWidgetInstance->AddToViewport();
        SubSystem->ClearAllMappings();
        SubSystem->AddMappingContext(MyMapping, 0);
        SubSystem->AddMappingContext(CameraMapping, 0);
    }
    cnt++;
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
        ViewHitLocation = HitLocation;

        //GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, "Spawn");

        StaticMeshActor->SetMobility(EComponentMobility::Movable);
        StaticMeshActor->GetStaticMeshComponent()->SetStaticMesh(MeshData.Mesh);
        DynamicMaterial = UMaterialInstanceDynamic::Create(StaticMeshActor->GetStaticMeshComponent()->GetMaterial(0), this);
        num--;
        ChangeView();

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

        //GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, "2");
        DynamicMaterial->SetTextureParameterValue("BaseColorTexture", TextureData.Texture); // Set texture parameter in material
        StaticMeshActor->GetStaticMeshComponent()->SetMaterial(0, DynamicMaterial);
    }
}

void AInteractiveArchController::Delete()
{
    if (WallSplineArr.Num() > 0) {
        WallSplineArr[CurrWall]->Destroy();
        WallSplineArr.RemoveAt(CurrWall);
        WallConstructionDelegate.ExecuteIfBound(FString("Current Wall Deleted"));

    }
    if (WallSplineArr.Num() <= 0) {
        WallSplineArr.Add(NewObject<AWallSpline>(this));
        CurrWall = WallSplineArr.Num() - 1;
        WallConstructionDelegate.ExecuteIfBound(FString("Current Wall Deleted"));

    }
    if (CurrWall >= WallSplineArr.Num() - 1) {
        CurrWall = WallSplineArr.Num() - 1;
    }

}

void AInteractiveArchController::DeleteAll()
{
    for (int i = 0; i < WallSplineArr.Num(); i++) {
        WallSplineArr[i]->Destroy();
    }
    WallConstructionDelegate.ExecuteIfBound(FString("All Walls Deleted"));

    WallSplineArr.Empty();
    WallSplineArr.Add(NewObject<AWallSpline>(this));
    CurrWall = WallSplineArr.Num() - 1;
}

void AInteractiveArchController::OnLeftClick(const FInputActionValue& ActionValue)
{
    FHitResult HitResult;
    GetHitResultUnderCursor(ECC_Visibility, true, HitResult);

    if (HitResult.bBlockingHit)
    {
        FVector ClickLocation = HitResult.Location;
        if (CurrWall < WallSplineArr.Num())
        {
            WallSplineArr[CurrWall]->SetPointLocation(ClickLocation);
        }
        if (WallSplineArr[CurrWall]->SpileArr.Num() == 1)
            WallConstructionDelegate.ExecuteIfBound("Construction Statrted");
        
    }
}

void AInteractiveArchController::OnRightClick(const FInputActionValue& ActionValue)
{
    if (WallSplineArr[CurrWall]->NoOfSplinePoints > 0) {
        AWallSpline* WallObj = NewObject<AWallSpline>(this);
        WallSplineArr.Add(WallObj);
        CurrWall = WallSplineArr.Num() - 1;

        WallConstructionDelegate.ExecuteIfBound(FString("Construction Completed"));
    }
}

void AInteractiveArchController::Undo(const FInputActionValue& ActionValue)
{
    if (CurrWall >= WallSplineArr.Num() - 1) {
        CurrWall = WallSplineArr.Num() - 1;
    }
    if (WallSplineArr.Num()) {
        if (WallSplineArr[CurrWall]->SpileArr.Num() > 0) {
            WallSplineArr[CurrWall]->SplineComponent->RemoveSplinePoint(WallSplineArr[CurrWall]->SpileArr.Num());
            WallSplineArr[CurrWall]->SpileArr[WallSplineArr[CurrWall]->SpileArr.Num() - 1]->DestroyComponent();
            WallSplineArr[CurrWall]->SpileArr.RemoveAt(WallSplineArr[CurrWall]->SpileArr.Num() - 1);
        }
        else {
            WallSplineArr[CurrWall]->SplineComponent->ClearSplinePoints();
            WallSplineArr[CurrWall]->SpileArr.Empty();
            WallSplineArr.RemoveAt(CurrWall);
        }
    }
    else {
        WallSplineArr.Add(NewObject<AWallSpline>(this));
    }
}

void AInteractiveArchController::OnLeft(const FInputActionValue& ActionValue)
{
    if (CurrWall != 0) {
        CurrWall--;
        WallConstructionDelegate.ExecuteIfBound(FString::Printf(TEXT("Now You Interact With Wall : %d"), CurrWall + 1));

    }
    else {
        WallConstructionDelegate.ExecuteIfBound(FString("No more prwevious wall press right"));
    }
}

void AInteractiveArchController::OnRight(const FInputActionValue& ActionValue)
{
    if (CurrWall != WallSplineArr.Num() - 1) {
        CurrWall++;
        WallConstructionDelegate.ExecuteIfBound(FString::Printf(TEXT("Now You Interact With Wall : %d"), CurrWall + 1));

    }
    else {
        WallConstructionDelegate.ExecuteIfBound(FString("No more next wall press left"));

    }
}