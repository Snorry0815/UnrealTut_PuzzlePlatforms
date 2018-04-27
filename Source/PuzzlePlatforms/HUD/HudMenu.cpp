// Fill out your copyright notice in the Description page of Project Settings.

#include "HudMenu.h"

#include "Components/Button.h"


void UHudMenu::Toggle()
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

void UHudMenu::Show()
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

void UHudMenu::Hide()
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

void UHudMenu::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	if (!IsInViewport())
		return;

	Hide();

	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
}


bool UHudMenu::Initialize()
{
	if (!Super::Initialize())
		return false;

	if (!ensure(cancelButton != nullptr))
		return false;

	cancelButton->OnClicked.AddDynamic(this, &UHudMenu::OnCancelClicked);

	if (!ensure(quitButton != nullptr))
		return false;

	quitButton->OnClicked.AddDynamic(this, &UHudMenu::OnQuitClicked);

	return true;
}

void UHudMenu::OnCancelClicked()
{
	Hide();
}

void UHudMenu::OnQuitClicked()
{
	FGenericPlatformMisc::RequestExit(false);
}
