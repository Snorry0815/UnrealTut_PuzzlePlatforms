// Fill out your copyright notice in the Description page of Project Settings.

#include "ServerFoundEntry.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "MainMenu.h"
#include "OnlineSessionSettings.h"

void UServerFoundEntry::Setup(UMainMenu* _parent, uint32 _index, const class FOnlineSessionSearchResult& searchResult)
{
	parent = _parent;
	index = _index;
	FString serverNameValue;
	searchResult.Session.SessionSettings.Get(TEXT("ServerName"), serverNameValue);
	serverName->SetText(FText::FromString(serverNameValue));
	serverOwnerName->SetText(FText::FromString(searchResult.Session.OwningUserName));
	int availableConnections = searchResult.Session.NumOpenPrivateConnections + searchResult.Session.NumOpenPublicConnections;
	int totalConnections = searchResult.Session.SessionSettings.NumPrivateConnections + searchResult.Session.SessionSettings.NumPublicConnections;
	
	numberOfPlayers->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), totalConnections - availableConnections, totalConnections)));

	if (!ensure(btnServerFoundEntry != nullptr))
		return;

	btnServerFoundEntry->OnClicked.AddDynamic(this, &UServerFoundEntry::OnButtonPressed);
}

void UServerFoundEntry::OnButtonPressed()
{
	parent->SelectIndex(index);
}
