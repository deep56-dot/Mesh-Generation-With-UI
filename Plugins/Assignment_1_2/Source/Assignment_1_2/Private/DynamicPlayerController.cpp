#include "DynamicPlayerController.h"


void ADynamicPlayerController::BeginPlay()
{
	Super::BeginPlay();
	PawnDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Assignment_1_2/DataTable/PawnDataTable.PawnDataTable'"));
	OnPressP();
}

void ADynamicPlayerController::OnPressP()
{

	if (CurrentPawn)
	{
		CurrentPawn->Destroy();
		CurrentPawn = nullptr;
		CurrentPawnIndex++;
	}
	if (PawnDataTable)
	{
		TArray<FName> RowNames = PawnDataTable->GetRowNames();
		if (RowNames.Num() == CurrentPawnIndex) {
			CurrentPawnIndex = 0;
		}

		FName RowNameByIndex = RowNames[CurrentPawnIndex];

		FPawnData* RowData = PawnDataTable->FindRow<FPawnData>(RowNameByIndex, TEXT(""));
		if (RowData)
		{

			TSubclassOf<APawn> CharacterClassToSpawn = RowData->PawnClass;
			if (CharacterClassToSpawn)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				FVector SpawnLocation_ = FVector(50, 50, 50);
				FRotator SpawnRotation = FRotator(0, 0, 0);

				APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(CharacterClassToSpawn, SpawnLocation_, SpawnRotation, SpawnParams);

				if (SpawnedPawn)
				{
					Possess(SpawnedPawn);
					GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Cyan, FString("Possessed spawned character"));
					CurrentPawn = SpawnedPawn;

					if (RowData->PawnType == EPawnType::TopDown) {
						SetShowMouseCursor(true);
						DisplayAttributes((Cast<ABaseCharacter>(SpawnedPawn))->PawnAssetData);
					}
					else if (RowData->PawnType == EPawnType::ThirdPerson) {
						SetShowMouseCursor(false);
						DisplayAttributes((Cast<ABaseCharacter>(SpawnedPawn))->PawnAssetData);
					}
					else {
						SetShowMouseCursor(false);
						DisplayAttributes((Cast<AFirstPersonPawn>(SpawnedPawn))->PawnAssetData);

					}
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString("Failed to spawn actor"));
				}
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString("Character class to spawn is invalid"));
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString("Failed to find row data in DataTable"));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString("Failed to load DataTable"));
	}
}


void ADynamicPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("ToggleAction", IE_Pressed, this, &ADynamicPlayerController::OnPressP);
}