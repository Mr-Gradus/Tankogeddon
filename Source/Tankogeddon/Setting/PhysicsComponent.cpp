// Fill out your copyright notice in the Description page of Project Settings.

#include "PhysicsComponent.h"


TArray<FVector> UPhysicsComponent::GenerateTrajectory(FVector StartPos, FVector Velocity, float MaxTime, float TimeStep, float MinZValue /*= 0*/)
{
	TArray<FVector> Trajectory;
	FVector GravityVector(0, 0, Gravity);
	for (float Time = 0; Time < MaxTime; Time = Time + TimeStep)
	{
		FVector Position = StartPos + Velocity * Time + GravityVector * Time * Time / 2;
		if (Position.Z <= MinZValue)
			break;

		Trajectory.Add(Position);
	}

	return Trajectory;

}