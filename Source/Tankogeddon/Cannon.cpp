// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Engine/Engine.h"

// Sets default values
ACannon::ACannon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent * sceeneCpm = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = sceeneCpm;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon mesh"));
    Mesh->SetupAttachment(RootComponent);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawnpoint"));
    ProjectileSpawnPoint->SetupAttachment(Mesh);
}


void ACannon::Fire()
{
	if(!ReadyToFire)
	{
		return;
	}
	ReadyToFire = false;
	
	AmmoCount--;
	if(AmmoCount == 0)
	{
		GEngine->AddOnScreenDebugMessage(10, 1,FColor::Red, "Reload");
		return;
	}
	
	if(Type == ECannonType::FireProjectile)
    {
    GEngine->AddOnScreenDebugMessage(10, 1,FColor::Green, "Fire - projectile");
    }
    else
    {
    GEngine->AddOnScreenDebugMessage(10, 1,FColor::Green, "Fire - trace");
    }

	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1 / FireRate, false);
}

void ACannon::FireSpecial()
{
	if(!ReadyToFire)
	{
		return;
	}
	ReadyToFire = false;

	if(Type == ECannonType::FireProjectile)
	{
		GEngine->AddOnScreenDebugMessage(10, 1,FColor::Yellow, "Fire - Special");
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(10, 1,FColor::Yellow, "Fire - trace");
	}

	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 2 / FireRate, false);
}

bool ACannon::IsReadyToFire()
{
	return ReadyToFire;
}

void ACannon::Reload()
{
	ReadyToFire = true;
}

void ACannon::BeginPlay()
{
	Super::BeginPlay();
	Reload();
}

