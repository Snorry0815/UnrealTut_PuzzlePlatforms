// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/HudMenu.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

namespace
{
	//const FName SESSION_NAME = TEXT("GameSession");
}

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer& objectinitializer)
	: UGameInstance(objectinitializer)
	, hudMenu(nullptr)
{
	DestroySessionComplete = FOnDestroySessionCompleteDelegate::CreateUObject(this, &UPuzzlePlatformsGameInstance::OnDestroySessionComplete);
	DestroyBeforeHostingComplete = FOnDestroySessionCompleteDelegate::CreateUObject(this, &UPuzzlePlatformsGameInstance::OnDestroySessionBeforeHostingComplete);

	static ConstructorHelpers::FClassFinder<UMainMenu> MainMenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (!ensure(MainMenuBPClass.Class != nullptr))
		return;

	MenuClass = MainMenuBPClass.Class;	
	
	static ConstructorHelpers::FClassFinder<UHudMenu> HudMenuBPClass(TEXT("/Game/MenuSystem/WBP_HudMenu"));
	if (!ensure(HudMenuBPClass.Class != nullptr))
		return;

	HudClass = HudMenuBPClass.Class;
}

void UPuzzlePlatformsGameInstance::NetworkError(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType,
	const FString& ErrorString)
{
	BackToMainMenu();
}

void UPuzzlePlatformsGameInstance::WorldDestroyed(UWorld* InWorld)
{
	hudMenu = nullptr;
}

void UPuzzlePlatformsGameInstance::ToggleHudMenu()
{
	if (hudMenu != nullptr)
	{
		hudMenu->Toggle();
	}
	else
	{
		if (!ensure(HudClass != nullptr))
			return;

		hudMenu = CreateWidget<UHudMenu>(this, HudClass);
		if (!ensure(hudMenu != nullptr))
			return;

		hudMenu->Show();
		hudMenu->SetMenuInterface(this);
	}
}


void UPuzzlePlatformsGameInstance::Init()
{
	IOnlineSubsystem* onlineSub = IOnlineSubsystem::Get();
	if (onlineSub != nullptr)
	{
		sessionInterface = onlineSub->GetSessionInterface();
		if (sessionInterface.IsValid())
		{
			sessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnCreateSessionComplete);
			sessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnFindSessionsComplete);
			sessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformsGameInstance::OnJoinSessionComplete);
			
			onlineSessionSearch = MakeShareable(new FOnlineSessionSearch());
		}
	}

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) 
		return;

	Engine->OnNetworkFailure().AddUObject(this, &UPuzzlePlatformsGameInstance::NetworkError);
	Engine->OnWorldDestroyed().AddUObject(this, &UPuzzlePlatformsGameInstance::WorldDestroyed);
}

void UPuzzlePlatformsGameInstance::LoadMainMenu()
{
	if (!ensure(MenuClass != nullptr))
		return;

	menu = CreateWidget<UMainMenu>(this, MenuClass);
	if (!ensure(menu != nullptr))
		return;

	menu->Show();
	menu->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::Host(const FString& serverName)
{
	if (!sessionInterface.IsValid())
		return;

	currentServerName = serverName;
	auto* alreadyExistingSession = sessionInterface->GetNamedSession(NAME_GameSession);
	if (alreadyExistingSession != nullptr)
	{
		sessionInterface->DestroySession(NAME_GameSession, DestroyBeforeHostingComplete);
	}
	else
	{
		StartSession();
	}
}

void UPuzzlePlatformsGameInstance::Join(const FString& adress)
{
	auto* localPlayer = GetFirstLocalPlayerController();
	if (!ensure(localPlayer != nullptr))
		return;
	
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString("Joining ") + adress);


	localPlayer->ClientTravel(adress, ETravelType::TRAVEL_Absolute);
}

void UPuzzlePlatformsGameInstance::JoinSelected(uint32 selectedIndex)
{
	if (!onlineSessionSearch.IsValid())
		return;

	const auto& results = onlineSessionSearch->SearchResults;
	if (!ensure(selectedIndex < static_cast<uint32>(results.Num())))
		return;

	const auto& selected = results[selectedIndex];
	if (!selected.IsValid())
		return;


	if (!sessionInterface.IsValid())
		return;

	sessionInterface->JoinSession(0, NAME_GameSession, selected);
}


void UPuzzlePlatformsGameInstance::RefreshServerList()
{
	if (refreshActive)
		return;

	if (onlineSessionSearch.IsValid())
	{
		refreshActive = true;
		onlineSessionSearch->MaxSearchResults = 100;
		onlineSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		sessionInterface->FindSessions(0, onlineSessionSearch.ToSharedRef());
	}
}

void UPuzzlePlatformsGameInstance::BackToMainMenu()
{
	APlayerController * playerController = GetFirstLocalPlayerController();
	if (!ensure(playerController != nullptr))
		return;
	
	playerController->ClientTravel("/Game/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
}

void UPuzzlePlatformsGameInstance::Quit()
{
	auto* world = GetWorld();
	if (!ensure(world != nullptr))
		return;
	
	if (world->IsPlayInEditor())
	{
		auto* localPlayer = GetFirstLocalPlayerController();
		if (!ensure(localPlayer != nullptr))
			return;

		localPlayer->ConsoleCommand("Quit");
	}
	else
	{
		FGenericPlatformMisc::RequestExit(false);
	}
}


void UPuzzlePlatformsGameInstance::QuitServer()
{
	IOnlineSubsystem* onlineSub = IOnlineSubsystem::Get();
	if (onlineSub)
	{
		IOnlineSessionPtr Sessions = onlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			Sessions->AddOnDestroySessionCompleteDelegate_Handle(DestroySessionComplete);
			Sessions->DestroySession(GameSessionName);
		}
	}
}

void UPuzzlePlatformsGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("SessionDestroyed"));
}


void UPuzzlePlatformsGameInstance::OnDestroySessionBeforeHostingComplete(FName SessionName, bool bWasSuccessful)
{
	StartSession();
}

void UPuzzlePlatformsGameInstance::StartSession()
{
	if (!sessionInterface.IsValid())
		return;

	FOnlineSessionSettings sessionSettings;
	
	sessionSettings.bIsLANMatch = (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL");

	sessionSettings.NumPublicConnections = 5;
	sessionSettings.bShouldAdvertise = true;
	sessionSettings.bUsesPresence = true;
	sessionSettings.Set(TEXT("ServerName"), currentServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	sessionInterface->CreateSession(0, NAME_GameSession, sessionSettings);
}

void UPuzzlePlatformsGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not Create Session!"));
		return;
	}

	auto* world = GetWorld();
	if (!ensure(world != nullptr))
		return;

	world->ServerTravel("/Game/Maps/Lobby?listen");

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Hosting after session complete"));
}

void UPuzzlePlatformsGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type type)
{
	FString connectInfo;
	if (!sessionInterface->GetResolvedConnectString(NAME_GameSession, connectInfo))
		return;

	auto* localPlayer = GetFirstLocalPlayerController();
	if (!ensure(localPlayer != nullptr))
		return;

	localPlayer->ClientTravel(connectInfo, ETravelType::TRAVEL_Absolute);
}

void UPuzzlePlatformsGameInstance::QuitClient()
{
}

void UPuzzlePlatformsGameInstance::OnFindSessionsComplete(bool success)
{
	refreshActive = false;

	if (!success || !onlineSessionSearch.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("OnFindSessionsComplete not OnFindSessionsComplete!"));
		return;
	}

	const auto& results = onlineSessionSearch->SearchResults;
	if (menu)
	{
		menu->SetServerList(results);
	}
}