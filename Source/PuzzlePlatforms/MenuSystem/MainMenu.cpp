// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Button.h"
#include "WidgetSwitcher.h"
#include "Engine/Engine.h"
#include "ServerFoundEntry.h"
#include "ScrollBox.h"
#include "ConstructorHelpers.h"
#include "Text.h"
#include "OnlineSessionSettings.h"

UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UServerFoundEntry> ServerFoundEntryBPClass(TEXT("/Game/MenuSystem/WBP_ServerFoundEntry"));
	if (!ensure(ServerFoundEntryBPClass.Class != nullptr))
		return;

	ServerFoundEntryClass = ServerFoundEntryBPClass.Class;
}

void UMainMenu::SetServerList(const TArray<FOnlineSessionSearchResult>& searchResults)
{
	ipAdressTarget->ClearChildren();
	for (const auto& searchResult : searchResults)
	{
		AddServerListEntry(searchResult.Session.GetSessionIdStr());
	}
}

void UMainMenu::AddServerListEntry(const FString& serverName)
{
	auto* world = this->GetWorld();
	if (!ensure(world != nullptr))
		return;

	auto* entry = CreateWidget<UServerFoundEntry>(world, ServerFoundEntryClass);
	if (!ensure(entry != nullptr))
		return;
	entry->SetText(FText::FromString(serverName));

	ipAdressTarget->AddChild(entry);
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
	
	if (!ensure(btnRefresh != nullptr))
		return false;
	btnRefresh->OnClicked.AddDynamic(this, &UMainMenu::OnRefreshServerList);

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
	OnRefreshServerList();

	menuSelecter->SetActiveWidget(joinMenu);
}

void UMainMenu::OnCancelJoinClicked()
{
	menuSelecter->SetActiveWidget(mainMenu);
}

void UMainMenu::OnJoinServer()
{
	if (menuInterface == nullptr)
		return;

	menuInterface->Join("");
}

void UMainMenu::OnQuit()
{
	if (menuInterface == nullptr)
		return;
	menuInterface->Quit();
}

void UMainMenu::OnRefreshServerList()
{
	if (menuInterface == nullptr)
		return;
	menuInterface->RefreshServerList();
}