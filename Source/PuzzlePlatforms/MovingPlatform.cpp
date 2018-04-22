// Fill out your copyright notice in the Description page of Project Settings.

#include "MovingPlatform.h"
#include "Engine/Engine.h"


AMovingPlatform::AMovingPlatform()
	: speed(50.0f)
	, distanceTraveled(0.0f)
	, direction(1.0f)
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
		globalStartLocation = GetActorLocation();
		globalTargetLocation = this->GetTransform().TransformPosition(targetLocation);
		movingVector = globalTargetLocation - globalStartLocation;
		totalTravelDistance = movingVector.Size();
		movingVector = movingVector.GetSafeNormal();
	}
}

void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!HasAuthority())
		return;

	if (activeTriggers > 0)
	{
		MovePlatform(DeltaTime);
	}
}

void AMovingPlatform::MovePlatform(float DeltaTime)
{
	auto currentLocation = this->GetActorLocation();

	distanceTraveled += speed * DeltaTime;
	if (distanceTraveled >= totalTravelDistance)
	{
		if (direction > 0.0f)
		{
			currentLocation = globalTargetLocation;
		}
		else
		{
			currentLocation = globalStartLocation;
		}

		distanceTraveled -= totalTravelDistance;
		direction *= -1.0f;
		currentLocation += movingVector * (direction * distanceTraveled);
	} 
	else
	{
		currentLocation += movingVector * (direction * speed * DeltaTime);
	}

	this->SetActorLocation(currentLocation);
}

void AMovingPlatform::AddActiveTrigger()
{
	++activeTriggers;
}

void AMovingPlatform::RemoveActiveTrigger()
{
	--activeTriggers;
}