// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Button.h"
#include "WidgetSwitcher.h"
#include "EditableText.h"
#include "Engine/Engine.h"

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