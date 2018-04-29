// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/HudMenu.h"
#include "OnlineSessionInterface.h"
#include "OnlineSubsystem.h"

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer& objectinitializer)
	: UGameInstance(objectinitializer)
	, hudMenu(nullptr)
{
	DestroySessionComplete = FOnDestroySessionCompleteDelegate::CreateUObject(this, &UPuzzlePlatformsGameInstance::OnDestroySessionComplete);

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

	auto* menu = CreateWidget<UMainMenu>(this, MenuClass);
	if (!ensure(menu != nullptr))
		return;

	menu->Show();
	menu->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::Host()
{
	auto* world = GetWorld();
	if (!ensure(world != nullptr))
		return;

	world->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Hosting"));
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

void UPuzzlePlatformsGameInstance::QuitClient()
{
}
