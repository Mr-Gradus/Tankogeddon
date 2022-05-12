// Fill out your copyright notice in the Description page of Project Settings.


#include "ParentTankTurret.h"
#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"


void AParentTankTurret::OnDeath()
{
	Destroy();
}

void AParentTankTurret::OnHealthChanged(float Health)
{
}

// Sets default values
AParentTankTurret::AParentTankTurret()
{
	PrimaryActorTick.bCanEverTick = true;

}


void AParentTankTurret::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters Params;
	Params.Owner = this;

	Cannon = GetWorld()->SpawnActor<ACannon>(CannonClass, Params);
	Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

}

void AParentTankTurret::FindBestTarget()
{
	float MinDistance = 5555555;
	BestTarget = nullptr;
	for(auto Target : Targets)
	{
		auto Distance = FVector::Dist2D(GetActorLocation(), Target->GetActorLocation());
		if (MinDistance > Distance)
		{
			MinDistance = Distance;
			BestTarget = Target;
		}
	}
}


