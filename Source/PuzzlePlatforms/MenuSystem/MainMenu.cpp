// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Button.h"
#include "WidgetSwitcher.h"
#include "EditableText.h"
#include "Engine/Engine.h"

void UMainMenu::SetMenuInterface(IMenuInterface* newMenuInterface)
{
	menuInterface = newMenuInterface;
}

void UMainMenu::Setup()
{
	auto* world = GetWorld();
	if (!ensure(world != nullptr))
		return;
	
	AddToViewport();

	auto* playerController = world->GetFirstPlayerController();
	if (!ensure(playerController != nullptr))
		return;

	FInputModeUIOnly inputMode;
	inputMode.SetWidgetToFocus(TakeWidget());
	inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	playerController->SetInputMode(inputMode);
	playerController->bShowMouseCursor = true;
}

void UMainMenu::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	RemoveFromViewport();

	auto* world = GetWorld();
	if (!ensure(world != nullptr))
		return;

	auto* playerController = world->GetFirstPlayerController();
	if (!ensure(playerController != nullptr))
		return;

	FInputModeGameOnly inputMode;
	playerController->SetInputMode(inputMode);
	playerController->bShowMouseCursor = false;

	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
}

bool UMainMenu::Initialize()
{
	if (!Super::Initialize())
		return false;

	if (!ensure(hostButton != nullptr))
		return false;
	
	hostButton->OnClicked.AddDynamic(this, &UMainMenu::OnHostClicked);

	if (!ensure(joinButton != nullptr))
		return false;

	joinButton->OnClicked.AddDynamic(this, &UMainMenu::OnJoinClicked);

	if (!ensure(cancelJoin != nullptr))
		return false;

	cancelJoin->OnClicked.AddDynamic(this, &UMainMenu::OnCancelJoinClicked);

	if (!ensure(joinWithIp != nullptr))
		return false;

	joinWithIp->OnClicked.AddDynamic(this, &UMainMenu::OnJoinServer);
	
	return true;
}

void UMainMenu::OnHostClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("OnHostClicked"));
	if (menuInterface == nullptr)
		return;
	
	menuInterface->Host();
}

void UMainMenu::OnJoinClicked()
{
	menuSelecter->SetActiveWidget(joinMenu);
}

void UMainMenu::OnCancelJoinClicked()
{
	menuSelecter->SetActiveWidget(mainMenu);
}

void UMainMenu::OnJoinServer()
{
	auto text = ipAdress->GetText();
	menuInterface->Join(text.ToString());
}