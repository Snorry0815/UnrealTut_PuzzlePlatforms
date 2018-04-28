// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameMenu.h"
#include "MainMenu.generated.h"
/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UInGameMenu
{
	GENERATED_BODY()

private:
	UFUNCTION()
	void OnHostClicked();

	UFUNCTION()
	void OnJoinClicked();

	UFUNCTION()
	void OnJoinServer();

	UFUNCTION()
	void OnCancelJoinClicked();


	UPROPERTY(meta = (BindWidget))
		class UButton* hostButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* joinButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* cancelJoin;

	UPROPERTY(meta = (BindWidget))
		class UButton* joinWithIp;

	UPROPERTY(meta = (BindWidget))
		class UWidgetSwitcher* menuSelecter;

	UPROPERTY(meta = (BindWidget))
		class UEditableText* ipAdress;

	UPROPERTY(meta = (BindWidget))
		class UWidget* joinMenu;

	UPROPERTY(meta = (BindWidget))
		class UWidget* mainMenu;

protected:
	virtual bool Initialize() override;
};
