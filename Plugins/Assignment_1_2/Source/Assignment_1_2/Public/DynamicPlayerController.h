// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FirstPersonPawn.h"
#include "GameFramework/PlayerController.h"
#include "BaseCharacter.h"
#include "PawnAttributeAsset.h"
#include "Components/SkeletalMeshComponent.h" 
#include "UObject/ObjectMacros.h"
#include "Engine/DataTable.h"
#include "DynamicPlayerController.generated.h"

UENUM(BlueprintType)
enum class EPawnType : uint8
{
	ThirdPerson UMETA(DisplayName = "Third Person"),
	TopDown UMETA(DisplayName = "Top Down"),
	FirstPerson UMETA(DisplayName = "First Person")
};

// Define struct for DataTable row
USTRUCT(BlueprintType)
struct FPawnData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPawnType PawnType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<APawn> PawnClass;
};


UCLASS()
class ASSIGNMENT_1_2_API ADynamicPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	/** Setup input actions and context mappings for player. */
	virtual void SetupInputComponent() override;

	//ADynamicPlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pawn Data")
	UDataTable* PawnDataTable;

	APawn* CurrentPawn;
	int8 CurrentPawnIndex;

	void OnPressP();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void DisplayAttributes(UPawnAttributeAsset* Asset);
};
