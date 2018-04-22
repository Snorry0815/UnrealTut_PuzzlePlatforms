// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MovingPlatform.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API AMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	AMovingPlatform();

	UPROPERTY(EditAnywhere)
	float speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyCategory", Meta = (MakeEditWidget = true)) 
	FVector targetLocation;

	void AddActiveTrigger();
	void RemoveActiveTrigger();

protected:
	virtual void BeginPlay() override;
	void MovePlatform(float DeltaTime);
	virtual void Tick(float DeltaTime) override;

private:
	FVector globalStartLocation;
	FVector globalTargetLocation;
	FVector movingVector;
	float totalTravelDistance;
	float distanceTraveled;
	float direction;

	UPROPERTY(EditAnywhere)
	int activeTriggers = 1;
};
