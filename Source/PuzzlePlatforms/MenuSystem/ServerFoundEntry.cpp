// Fill out your copyright notice in the Description page of Project Settings.

#include "ServerFoundEntry.h"
#include "Components/TextBlock.h"

void UServerFoundEntry::SetText(const FText& inText)
{
	serverName->SetText(inText);
}
