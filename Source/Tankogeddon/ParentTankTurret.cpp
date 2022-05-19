// Fill out your copyright notice in the Description page of Project Settings.


#include "ParentTankTurret.h"

#include "DrawDebugHelpers.h"
#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void AParentTankTurret::Fire()
{
	if (Cannon)
	{
		Cannon->Fire();
	}
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
/*
void AParentTankTurret::Destroyed()
{
	Destroy();
}
*/
void AParentTankTurret::OnHealthChanged(float Health)
{
}

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

FVector AParentTankTurret::GetEyesPosition()
{
	return CannonSetupPoint->GetComponentLocation();
}
