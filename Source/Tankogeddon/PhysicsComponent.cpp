// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsComponent.h"

TArray<FVector> UPhysicsComponent::GenerateTrajectory(FVector StartPos, FVector Velocity, float MaxTime, float TimeStep, float MinZValue)
{
	TArray<FVector> Trajectory;
	FVector GravityVector(0,0,Gravity);
	for(float Time = 0; Time < MaxTime; Time = Time + TimeStep)
	{
		FVector Position = StartPos + Velocity * Time + GravityVector * Time * Time /
		2;
		if(Position.Z <= MinZValue)
			break;
		Trajectory.Add(Position);
}
	return Trajectory;
}


/*
// Sets default values for this component's properties
UPhysicsComponent::UPhysicsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPhysicsComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPhysicsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
*/
