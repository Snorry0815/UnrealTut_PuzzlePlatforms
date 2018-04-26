// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformsGameInstance.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "PlatformTrigger.h"
#include "MenuSystem/MainMenu.h"

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer& objectinitializer)
	: UGameInstance(objectinitializer)
{
	static ConstructorHelpers::FClassFinder<UMainMenu> MainMenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (!ensure(MainMenuBPClass.Class != nullptr))
		return;

	MenuClass = MainMenuBPClass.Class;
}

void UPuzzlePlatformsGameInstance::LoadMenu()
{
	if (!ensure(MenuClass != nullptr))
		return;
	
	auto* menu = CreateWidget<UMainMenu>(this, MenuClass);
	if (!ensure(menu != nullptr))
		return;

	menu->Setup();
	menu->SetMenuInterface(this);
}

void UPuzzlePlatformsGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("Init"));
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
