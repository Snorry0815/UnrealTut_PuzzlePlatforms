// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameMenu.h"
#include "HudMenu.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UHudMenu : public UInGameMenu
{
	GENERATED_BODY()
protected:
	virtual bool Initialize() override;

private:
	UFUNCTION()
	void OnCancelClicked();

	UFUNCTION()
	void OnQuitClicked();

	UPROPERTY(meta = (BindWidget))
		class UButton* cancelButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* quitButton;

};
