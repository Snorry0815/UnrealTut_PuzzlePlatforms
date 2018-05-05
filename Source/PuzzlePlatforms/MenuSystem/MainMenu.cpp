// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Button.h"
#include "WidgetSwitcher.h"
#include "Engine/Engine.h"
#include "ServerFoundEntry.h"
#include "ScrollBox.h"
#include "ConstructorHelpers.h"
#include "Text.h"
#include "EditableTextBox.h"
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
	wsResultField->SetActiveWidget(ipAdressTarget);
	ipAdressTarget->ClearChildren();
	selectedIndex.Reset();

	uint32 index = 0;
	for (const auto& searchResult : searchResults)
	{
		if (!searchResult.IsValid())
			continue;

		AddServerListEntry(searchResult, index);
		++index;
	}
}

void UMainMenu::AddServerListEntry(const FOnlineSessionSearchResult& searchResult, uint32 index)
{
	auto* world = this->GetWorld();
	if (!ensure(world != nullptr))
		return;

	auto* entry = CreateWidget<UServerFoundEntry>(world, ServerFoundEntryClass);
	if (!ensure(entry != nullptr))
		return;

	entry->Setup(this, index, searchResult);

	ipAdressTarget->AddChild(entry);
}

void UMainMenu::SetSelectedForIndex(uint32 index, bool value) const
{
	for (auto i = 0; i < ipAdressTarget->GetChildrenCount(); ++i)
	{
		auto* entry = ipAdressTarget->GetChildAt(i);
		auto* serverFoundEntry = dynamic_cast<UServerFoundEntry*>(entry);
		if (serverFoundEntry == nullptr)
			continue;

		if (serverFoundEntry->GetIndex() == index)
		{
			serverFoundEntry->isSelected = value;
			return;
		}
	}
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

	if (!ensure(btnCancelHostSettings != nullptr))
		return false;
	btnCancelHostSettings->OnClicked.AddDynamic(this, &UMainMenu::OnCancelHostSettingsClicked);

	if (!ensure(btnStartServerWithSettings != nullptr))
		return false;
	btnStartServerWithSettings->OnClicked.AddDynamic(this, &UMainMenu::OnStartServerWithSettingsClicked);

	return true;
}

void UMainMenu::OnHostClicked()
{
	menuSelecter->SetActiveWidget(hostSettings);
}

void UMainMenu::OnCancelHostSettingsClicked()
{

	menuSelecter->SetActiveWidget(mainMenu);
}

void UMainMenu::OnStartServerWithSettingsClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("OnHostClicked"));
	if (menuInterface == nullptr)
		return;
	FText name = serverNameField->GetText();
	menuInterface->Host(name.ToString());
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

	if (!selectedIndex.IsSet())
		return;

	menuInterface->JoinSelected(selectedIndex.GetValue());
}

void UMainMenu::OnQuit()
{
	if (menuInterface == nullptr)
		return;
	menuInterface->Quit();
}

void UMainMenu::SelectIndex(uint32 index)
{
	if (selectedIndex.IsSet())
		SetSelectedForIndex(selectedIndex.GetValue(), false);

	selectedIndex = index;
	SetSelectedForIndex(selectedIndex.GetValue(), true);
}

void UMainMenu::OnRefreshServerList()
{
	if (menuInterface == nullptr)
		return;

	wsResultField->SetActiveWidget(refreshingIndicator);
	menuInterface->RefreshServerList();
}