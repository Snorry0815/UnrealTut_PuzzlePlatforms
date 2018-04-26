// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetMenuInterface(IMenuInterface* newMenuInterface);

	void Setup();
	virtual  void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;

	UPROPERTY(meta = (BindWidget))
	class UButton* hostButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* joinButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* joinWithIp;
	
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* menuSelecter;

	UPROPERTY(meta = (BindWidget))
	class UEditableText* ipAdress;

protected:
	virtual bool Initialize() override;

private:
	UFUNCTION()
	void OnHostClicked();

	UFUNCTION()
	void OnJoinClicked();

	UFUNCTION()
	void OnJoinServer();

	IMenuInterface* menuInterface;
};
