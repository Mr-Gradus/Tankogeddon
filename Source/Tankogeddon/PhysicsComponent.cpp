// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsComponent.h"
#include "Math/UnrealMathUtility.h"

UPhysicsComponent::UPhysicsComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UPhysicsComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPhysicsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}

	FVector NewActorLocation = Owner->GetActorLocation() + Velocity * DeltaTime - FVector::UpVector * Gravity * FMath::Square(DeltaTime) / 2.f;
	Velocity += -FVector::UpVector * Gravity * DeltaTime;
	Owner->SetActorLocation(NewActorLocation, true);
}
