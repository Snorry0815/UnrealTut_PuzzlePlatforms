// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Button.h"
#include "WidgetSwitcher.h"
#include "Engine/Engine.h"
#include "ServerFoundEntry.h"
#include "ScrollBox.h"
#include "ConstructorHelpers.h"

UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UServerFoundEntry> ServerFoundEntryBPClass(TEXT("/Game/MenuSystem/WBP_ServerFoundEntry"));
	if (!ensure(ServerFoundEntryBPClass.Class != nullptr))
		return;

	ServerFoundEntryClass = ServerFoundEntryBPClass.Class;
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

	if (!ensure(btnQuit != nullptr))
		return false;

	btnQuit->OnClicked.AddDynamic(this, &UMainMenu::OnQuit);

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
	auto* world = this->GetWorld();
	if (!ensure(world != nullptr))
		return;

	auto* entry = CreateWidget<UServerFoundEntry>(world, ServerFoundEntryClass);
	if (!ensure(entry != nullptr))
		return;

	ipAdressTarget->AddChild(entry);
}

void UMainMenu::OnQuit()
{
	menuInterface->Quit();
}
