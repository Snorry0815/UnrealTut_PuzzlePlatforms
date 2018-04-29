// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameMenu.h"




void UInGameMenu::SetMenuInterface(IMenuInterface* newMenuInterface)
{
	menuInterface = newMenuInterface;
}

void UInGameMenu::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	if (IsInViewport())
		Hide();

	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
}

void UInGameMenu::Hide()
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
}

void UInGameMenu::Toggle()
{
	if (IsInViewport())
	{
		Hide();
	}
	else
	{
		Show();
	}
}

void UInGameMenu::Show()
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