// Fill out your copyright notice in the Description page of Project Settings.

#include "ServerFoundEntry.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "MainMenu.h"

void UServerFoundEntry::Setup(UMainMenu* _parent, uint32 _index, const FText& inText)
{
	parent = _parent;
	index = _index;
	serverName->SetText(inText);

	if (!ensure(btnServerFoundEntry != nullptr))
		return;

	btnServerFoundEntry->OnClicked.AddDynamic(this, &UServerFoundEntry::OnButtonPressed);
}

void UServerFoundEntry::OnButtonPressed()
{
	parent->SelectIndex(index);
}
