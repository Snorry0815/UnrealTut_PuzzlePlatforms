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
public:
	UMainMenu(const FObjectInitializer& ObjectInitializer);

	void SetServerList(const TArray<FOnlineSessionSearchResult>& searchResults);
	void SelectIndex(uint32 index);

private:
	void AddServerListEntry(const class FOnlineSessionSearchResult& searchResult, uint32 index);
	void SetSelectedForIndex(uint32 index, bool value) const;

	UFUNCTION()
	void OnHostClicked();

	UFUNCTION()
	void OnJoinClicked();

	UFUNCTION()
	void OnJoinServer();

	UFUNCTION()
	void OnQuit();

	UFUNCTION()
	void OnRefreshServerList();

	UFUNCTION()
	void OnCancelJoinClicked();

	UFUNCTION()
	void OnCancelHostSettingsClicked();

	UFUNCTION()
	void OnStartServerWithSettingsClicked();


	UPROPERTY(meta = (BindWidget))
	class UButton* hostButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* joinButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* cancelJoin;

	UPROPERTY(meta = (BindWidget))
	class UButton* btnRefresh;

	UPROPERTY(meta = (BindWidget))
	class UButton* joinWithIp;

	UPROPERTY(meta = (BindWidget))
	class UButton* btnQuit;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* menuSelecter;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* wsResultField;

	UPROPERTY(meta = (BindWidget))
	class UWidget* refreshingIndicator;
	
	UPROPERTY(meta = (BindWidget))
	class UWidget* joinMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* mainMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* hostSettings;
	
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* serverNameField;

	UPROPERTY(meta = (BindWidget))
	class UButton* btnStartServerWithSettings;

	UPROPERTY(meta = (BindWidget))
	class UButton* btnCancelHostSettings;


	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ipAdressTarget;

	TSubclassOf<class UServerFoundEntry> ServerFoundEntryClass;

protected:
	virtual bool Initialize() override;

	TOptional<uint32> selectedIndex;
};
