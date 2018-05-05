// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerFoundEntry.generated.h"

class UMainMenu;

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UServerFoundEntry : public UUserWidget
{
	GENERATED_BODY()
public:
	void Setup(UMainMenu* _parent, uint32 _index, const FText& inText);

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* serverName;
	uint32 index; 

	UPROPERTY()
	UMainMenu* parent;

	UPROPERTY(meta = (BindWidget))
	class UButton* btnServerFoundEntry;

	UFUNCTION()
	void OnButtonPressed();
};
