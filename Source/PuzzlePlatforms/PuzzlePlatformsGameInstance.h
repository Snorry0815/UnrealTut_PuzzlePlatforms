// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"
#include "DestroySessionCallbackProxy.h"
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
	virtual void Host() override;

	UFUNCTION(Exec)
	virtual void Join(const FString& adress) override;

	UFUNCTION(Exec)
	virtual void BackToMainMenu() override;

	UFUNCTION(Exec)
	virtual void Quit() override;

private:
	FOnDestroySessionCompleteDelegate DestroySessionComplete;
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	void QuitServer();
	void QuitClient();

	TSubclassOf<class UUserWidget> MenuClass;
	TSubclassOf<class UUserWidget> HudClass;

	class UHudMenu* hudMenu;
};
