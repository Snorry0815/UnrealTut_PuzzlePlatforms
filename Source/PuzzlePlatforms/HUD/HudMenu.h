// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HudMenu.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UHudMenu : public UUserWidget
{
	GENERATED_BODY()
	

	UPROPERTY(meta = (BindWidget))
	class UButton* cancelButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* quitButton;

public:
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;

	void Toggle();
	void Show();
	void Hide();

protected:
	virtual bool Initialize() override;

private:
	UFUNCTION()
	void OnCancelClicked();

	UFUNCTION()
	void OnQuitClicked();
	
};
