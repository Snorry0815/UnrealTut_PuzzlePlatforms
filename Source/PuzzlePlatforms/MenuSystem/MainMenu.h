// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	class UButton* hostButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* joinButton;

protected:
	virtual bool Initialize() override;

private:
	UFUNCTION()
	void OnHostClicked();
};
