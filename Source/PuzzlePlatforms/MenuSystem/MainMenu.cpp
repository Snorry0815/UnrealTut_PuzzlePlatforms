// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "Button.h"
#include "Engine/Engine.h"

bool UMainMenu::Initialize()
{
	if (!Super::Initialize())
		return false;

	if (!ensure(hostButton != nullptr))
		return false;
	
	hostButton->OnClicked.AddDynamic(this, &UMainMenu::OnHostClicked);

	return true;
}

void UMainMenu::OnHostClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("OnHostClicked"));
}