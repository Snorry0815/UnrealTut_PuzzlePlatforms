// Fill out your copyright notice in the Description page of Project Settings.

#include "MovingPlatform.h"


AMovingPlatform::AMovingPlatform()
	: speed(50.0f)
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
	SetMobility(EComponentMobility::Movable);
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);
	}
}

void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (HasAuthority())
	{
		auto currentLocation = this->GetActorLocation();
		currentLocation.X += speed * DeltaTime;
		this->SetActorLocation(currentLocation);	
	}
}
