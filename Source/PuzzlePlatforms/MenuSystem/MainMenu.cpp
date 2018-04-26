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
	//inputMode.SetWidgetToFocus();
	//inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
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
	joinButton->OnClicked.AddDynamic(this, &UMainMenu::OnJoinClicked);
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
	menuSelecter->SetActiveWidgetIndex(1);
}

void UMainMenu::OnJoinServer()
{
	auto text = ipAdress->GetText();
	menuInterface->Join(text.ToString());
}