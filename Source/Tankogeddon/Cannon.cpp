// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"

// Sets default values
ACannon::ACannon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);

	SpawnPoint = CreateDefaultSubobject<UArrowComponent>("SpawnPoint");
	SpawnPoint->SetupAttachment(RootComponent);
}

void ACannon::Fire()
{
	if (bReadyToFire)
		return;
	
	switch (Type)
	{
	case ECannonType::Projectile:
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("Projectile")));
		break;
	case ECannonType::Trace:
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("Projectile")));
		break;
	}
	bReadyToFire = false;
	GetWorld()->GetTimerManager().SetTimer(ReloadHandle, this, &ACannon::OnReload, 1/FireRate, false);
}

// Called when the game starts or when spawned
void ACannon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACannon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto Remaining = GetWorld()->GetTimerManager().GetTimerRemaining(ReloadHandle);

	GEngine->AddOnScreenDebugMessage(87348, -1, FColor::Blue, FString::Printf(TEXT("Until Reload %f"), Remaining));
}

void ACannon::OnReload()
{
	bReadyToFire = true;
}

