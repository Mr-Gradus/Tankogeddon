// Fill out your copyright notice in the Description page of Project Settings.


#include "ParentTankTurret.h"

#include "DrawDebugHelpers.h"
#include "GameHUD.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "HealthWidget.h"
#include "Components/WidgetComponent.h"


void AParentTankTurret::Fire()
{
	if (Cannon)
	{
		Cannon->Fire();
	}
}

void AParentTankTurret::TakeDamage(const FDamageInfo DamageInfo)
{
	HealthComponent->TakeDamage(DamageInfo);

}

void AParentTankTurret::Death()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DestructObject, GetActorTransform());
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DestructSound, GetActorLocation());
	
	if (AmmoboxClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.bNoFail = true;
		GetWorld()->SpawnActor<AAmmoBox>(AmmoboxClass, GetActorTransform(), SpawnParams);
	}
	Destroy();
}

void AParentTankTurret::OnHealthChanged(float Health)
{
	GEngine->AddOnScreenDebugMessage(3, 2.0f, FColor::Purple, FString::Printf(TEXT("%s HP: %f "), *GetName(), Health));

}


AParentTankTurret::AParentTankTurret()
{
	PrimaryActorTick.bCanEverTick = true;

	TankPawnProgressBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HP Bar"));
	TankPawnProgressBar->SetupAttachment(BodyMesh);

}


void AParentTankTurret::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	TargetController = Cast<ITargetController>(NewController);
}

void AParentTankTurret::SetHealth()
{
	if (Cast<UHealthWidget>(TankPawnProgressBar->GetUserWidgetObject()))
	{
		UHealthWidget* HP = Cast<UHealthWidget>(TankPawnProgressBar->GetUserWidgetObject());
		HP->SetHealthValue(HealthComponent->GetHealthPercent());
	}
}

void AParentTankTurret::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters Params;
	Params.Owner = this;

	Cannon = GetWorld()->SpawnActor<ACannon>(CannonClass, Params);
	Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	}

FVector AParentTankTurret::GetEyesPosition() const
{
	return CannonSetupPoint->GetComponentLocation();
}


ACannon* AParentTankTurret::GetCannon() const
{
	return Cannon;
}

UHealthComponent* AParentTankTurret::GetHealthComponent()
{
	return HealthComponent;
}