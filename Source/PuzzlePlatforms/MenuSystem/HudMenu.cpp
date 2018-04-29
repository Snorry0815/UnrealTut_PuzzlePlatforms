// Fill out your copyright notice in the Description page of Project Settings.

#include "HudMenu.h"

#include "Components/Button.h"

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
	menuInterface->BackToMainMenu();
}
