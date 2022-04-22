// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"

#include "DrawDebugHelpers.h"
#include "Projectile.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Engine/Engine.h"

// Sets default values
ACannon::ACannon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent * sceeneCpm = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = sceeneCpm;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon mesh"));
	Mesh->SetupAttachment(RootComponent);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawn point"));
	ProjectileSpawnPoint->SetupAttachment(Mesh);
}


void ACannon::Fire()
{
	if(!ReadyToFire)
	{
		return;	
	}
	ReadyToFire = false;

	if(Type == ECannonType::FireProjectile)
	{

		GEngine->AddOnScreenDebugMessage(-1, 1,FColor::Green, "Fire - projectile");

		FTransform projectileTransform(ProjectileSpawnPoint->GetComponentRotation(),
		ProjectileSpawnPoint->GetComponentLocation(), FVector(1));

		AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass,
		                                                              ProjectileSpawnPoint->GetComponentLocation(),
		                                                              ProjectileSpawnPoint->GetComponentRotation());
/*
		Ammo--;
		GEngine->AddOnScreenDebugMessage(-1, 1,FColor::Blue, "Ammo" + FString::FromInt(Ammo));
		if (Ammo == 0)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1,FColor::Red, "Need reload");
			return;
		}
*/
		if(projectile)
		{
			projectile->Start();		
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1,FColor::Green, "Fire - trace");
		FHitResult hitResult;
		FCollisionQueryParams traceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
		traceParams.bTraceComplex = true;
		traceParams.bReturnPhysicalMaterial = false;
		
		FVector start = ProjectileSpawnPoint->GetComponentLocation();
		FVector end = ProjectileSpawnPoint->GetForwardVector() * FireRange + start;
		if(GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_Visibility, traceParams))
		{
			DrawDebugLine(GetWorld(), start, hitResult.Location, FColor::Red, false, 0.5f, 0, 5);
			if(hitResult.Actor.Get())
			{
				hitResult.Actor.Get()->Destroy();
			}
		}
		else
		{
			DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 0.5f, 0, 5);
		}
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

