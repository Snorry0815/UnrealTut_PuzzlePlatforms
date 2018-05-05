// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"
#include "DestroySessionCallbackProxy.h"
#include "OnlineSessionInterface.h"
#include "MenuSystem/ServerFoundEntry.h"
#include "PuzzlePlatformsGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UPuzzlePlatformsGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:
	UPuzzlePlatformsGameInstance(const FObjectInitializer& objectinitializer);

	void NetworkError(UWorld* World, class UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);
	void WorldDestroyed(class UWorld* InWorld);
	
protected:
	virtual void Init() override;

	UFUNCTION(Exec, BlueprintCallable, Category = "MainMenu")
	void LoadMainMenu();

	UFUNCTION(Exec, BlueprintCallable, Category = "MainMenu")
	void ToggleHudMenu();

	UFUNCTION(Exec)
	virtual void Host(const FString& serverName) override;

	UFUNCTION(Exec)
	virtual void Join(const FString& adress) override;

	UFUNCTION(Exec)
	virtual void JoinSelected(uint32 selectedIndex) override;

	UFUNCTION(Exec)
	virtual void RefreshServerList() override;

	UFUNCTION(Exec)
	virtual void BackToMainMenu() override;

	UFUNCTION(Exec)
	virtual void Quit() override;

private:
	FOnDestroySessionCompleteDelegate DestroySessionComplete;
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	FOnDestroySessionCompleteDelegate DestroyBeforeHostingComplete;
	void OnDestroySessionBeforeHostingComplete (FName SessionName, bool bWasSuccessful);

	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type type);

	void StartSession();

	void QuitServer();
	void QuitClient();

	void OnFindSessionsComplete(bool success);

	TSubclassOf<class UUserWidget> MenuClass;
	TSubclassOf<class UUserWidget> HudClass;

	TSharedPtr<class FOnlineSessionSearch> onlineSessionSearch;
	class UHudMenu* hudMenu; 
	class UMainMenu* menu;

	IOnlineSessionPtr sessionInterface;
	bool refreshActive;
	FString currentServerName;
};
