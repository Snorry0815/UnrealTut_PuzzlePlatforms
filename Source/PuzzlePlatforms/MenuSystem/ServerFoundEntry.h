// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerFoundEntry.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UServerFoundEntry : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ipAdress;
};
