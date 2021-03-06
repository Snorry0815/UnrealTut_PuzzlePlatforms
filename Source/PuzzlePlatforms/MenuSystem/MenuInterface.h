// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MenuInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMenuInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PUZZLEPLATFORMS_API IMenuInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Host(const FString& serverName) = 0;
	virtual void RefreshServerList() = 0;
	virtual void Join(const FString& ipAdress) = 0;
	virtual void JoinSelected(uint32 selectedIndex) = 0;
	virtual void BackToMainMenu() = 0; 
	virtual void Quit() = 0;
};
