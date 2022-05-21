// Fill out your copyright notice in the Description page of Project Settings.


#include "Waypoint.h"

AWaypoint::AWaypoint()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AWaypoint::BeginPlay()
{
	Super::BeginPlay();
}

void AWaypoint::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

