// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameMode.h"
#include "Engine/World.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "OnlineSessionInterface.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	++currentPlayerCount;

	if (timerStarted)
		return;

	if (currentPlayerCount != targetPlayerCount)
		return;

	GetWorldTimerManager().SetTimer(startDelayTimer, this, &ALobbyGameMode::TravelToGame, 5.0f, false);
	timerStarted = true;
}

void ALobbyGameMode::TravelToGame()
{
	IOnlineSubsystem* onlineSub = IOnlineSubsystem::Get();
	if (onlineSub != nullptr)
	{
		auto sessionInterface = onlineSub->GetSessionInterface();
		if (sessionInterface.IsValid())
		{
			sessionInterface->StartSession(NAME_GameSession);
		}
	}

	auto* world = GetWorld();
	if (!ensure(world != nullptr))
		return;

	bUseSeamlessTravel = true;
	world->ServerTravel("/Game/Maps/Game?listen");
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	check(currentPlayerCount > 0);
	--currentPlayerCount;

	Super::Logout(Exiting);
}
