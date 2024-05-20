// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/TextBlock.h>
#include "MyWidget.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENT4_API UMyWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnyWhere,meta = (BindWidget))
	UTextBlock* Text;
};
