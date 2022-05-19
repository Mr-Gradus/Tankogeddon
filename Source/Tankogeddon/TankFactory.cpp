// Fill out your copyright notice in the Description page of Project Settings.


#include "TankFactory.h"

#include <ThirdParty/PhysX3/NvCloth/include/NvCloth/Allocator.h>

#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

ATankFactory::ATankFactory()
{
	PrimaryActorTick.bCanEverTick = false;
	
	USceneComponent * sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = sceneComp;

	BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Building Mesh"));
	BuildingMesh->SetupAttachment(sceneComp);

	TankSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon setup point"));
	TankSpawnPoint->AttachToComponent(sceneComp, FAttachmentTransformRules::KeepRelativeTransform);

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
	HitCollider->SetupAttachment(sceneComp);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health component"));
	HealthComponent->OnDeath.AddUObject(this, &ATankFactory::Death);
	HealthComponent->OnHealthChanged.AddUObject(this, &ATankFactory::DamageTaked);
	
}

void ATankFactory::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle TargetingTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TargetingTimerHandle, this, &ATankFactory::SpawnNewTank, SpawnTankRate, true, 3);

	
	
}

void ATankFactory::TakeDamage(FDamageInfo DamageInfo)
{
	HealthComponent->TakeDamage(DamageInfo);
}
void ATankFactory::Death()
{
	Destroy();
}

void ATankFactory::DamageTaked(float DamageValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Factory %s taked damage:%f Health:%f"),
	*GetName(), DamageValue, HealthComponent->GetHealth());
}




void ATankFactory::SpawnNewTank()
{
	if(SpawnTankClass)
	{
		FTransform SpawnTransform(TankSpawnPoint->GetComponentRotation(), TankSpawnPoint->GetComponentLocation(), FVector(1));
		ATankPawn * NewTank = GetWorld()->SpawnActorDeferred<ATankPawn>(SpawnTankClass, SpawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		//
		NewTank->WaypointTag;                           // SetPatrollingPoints(TankWayPoints);
		//
		UGameplayStatics::FinishSpawningActor(NewTank, SpawnTransform);
	}
}