// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsProjectile.h"
#include "PhysicsComponent.h"
#include <Particles/ParticleSystemComponent.h>
#include <DrawDebugHelpers.h>
#include <Kismet/KismetMathLibrary.h>

#include "Kismet/GameplayStatics.h"

APhysicsProjectile::APhysicsProjectile()
{
	PhysicsComponent = CreateDefaultSubobject<UPhysicsComponent>(TEXT("PhysicsComponent"));

	TrailEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trail effect"));
	TrailEffect->SetupAttachment(RootComponent);
}

void APhysicsProjectile::Start()
{
	MoveVector = GetActorForwardVector() * TrajectorySimulationSpeed;
	CurrentTrajectory = PhysicsComponent->GenerateTrajectory(GetActorLocation(), MoveVector, TrajectorySimulationMaxTime, TrajectorySimulationTimeStep, 0);
	if (bShowTrajectory)
	{
		for (FVector Position : CurrentTrajectory)
		{
			DrawDebugSphere(GetWorld(), Position, 5, 8, FColor::Yellow, true, 1, 0, 2);
		}
	}

	TrajectoryPointIndex = 0;
	Super::Start();
}


void APhysicsProjectile::Move()
{
	FVector CurrentMoveVector = CurrentTrajectory[TrajectoryPointIndex] - GetActorLocation();
	CurrentMoveVector.Normalize();
	FVector NewLocation = GetActorLocation() + CurrentMoveVector * MoveSpeed * MoveRate;
	SetActorLocation(NewLocation);
	if (FVector::Distance(NewLocation, CurrentTrajectory[TrajectoryPointIndex]) <= MoveAccurency)
	{
		TrajectoryPointIndex++;
		if (TrajectoryPointIndex >= CurrentTrajectory.Num())
		{
			
			Explode();
			Destroy();
		}
		else
		{
			FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CurrentTrajectory[TrajectoryPointIndex]);
			SetActorRotation(NewRotation);
		}
	}
}





