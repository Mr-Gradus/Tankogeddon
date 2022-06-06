// Fill out your copyright notice in the Description page of Project Settings.


#include "TankFactory.h"

#include "HealthWidget.h"
#include "MapLoader.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

ATankFactory::ATankFactory()
{
	PrimaryActorTick.bCanEverTick = false;
	
	USceneComponent * SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneComp;

	BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Building Mesh"));
	BuildingMesh->SetupAttachment(SceneComp);

	TankSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon setup point"));
	TankSpawnPoint->AttachToComponent(SceneComp, FAttachmentTransformRules::KeepRelativeTransform);

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
	HitCollider->SetupAttachment(SceneComp);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health component"));
	HealthComponent->OnDeath.AddUObject(this, &ATankFactory::Death);
	HealthComponent->OnHealthChanged.AddUObject(this, &ATankFactory::DamageTaked);

	ProgressBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HP Bar"));
	ProgressBarWidgetComponent->SetupAttachment(BuildingMesh);

}

void ATankFactory::BeginPlay()
{
	Super::BeginPlay();

	if(LinkedMapLoader)
		LinkedMapLoader->SetIsActivated(false);

	GetWorld()->GetTimerManager().SetTimer(TargetingTimerHandle, this, &ATankFactory::SpawnNewTank, SpawnTankRate, true, 3);

	SetHealth();
}

void ATankFactory::TakeDamage(const FDamageInfo DamageInfo)
{
	HealthComponent->TakeDamage(DamageInfo);

	SetHealth();
}

void ATankFactory::Death()
{
	if(LinkedMapLoader)
		LinkedMapLoader->SetIsActivated(true);
	Destroy();
	GetWorld()->GetTimerManager().ClearTimer(TargetingTimerHandle);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DestuctFactory, GetActorTransform());
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DestructSound, GetActorLocation());
}

void ATankFactory::DamageTaked(const float DamageValue) const
{
	//UE_LOG(LogTemp, Warning, TEXT("Factory %s taked damage:%f Health:%f"), *GetName(), DamageValue, HealthComponent->GetHealth());

	
}

void ATankFactory::SpawnNewTank()
{
	if(SpawnTankClass)
	{
		const FTransform SpawnTransform(TankSpawnPoint->GetComponentRotation(), TankSpawnPoint->GetComponentLocation(), FVector(1));

		ATankPawn * NewTank = GetWorld()->SpawnActorDeferred<ATankPawn>(SpawnTankClass, SpawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		
		NewTank->WaypointTag = WaypointTag;
		
		UGameplayStatics::FinishSpawningActor(NewTank, SpawnTransform);

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SpawnParticleSystem, SpawnTransform);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SpawnSound, GetActorLocation());
	}
}

void ATankFactory::SetHealth()
{
	if(IsValid(ProgressBarWidgetComponent))
	{
		const UHealthWidget* HealthWidget = Cast<UHealthWidget>(ProgressBarWidgetComponent->GetUserWidgetObject());
		
		if (IsValid(HealthWidget))
		{
			HealthWidget->SetHealthValue(GetHealthComponent()->GetHealthPercent());
		}
	}
}

UHealthComponent* ATankFactory::GetHealthComponent()
{
	return HealthComponent;
}