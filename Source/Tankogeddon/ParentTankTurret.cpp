// Fill out your copyright notice in the Description page of Project Settings.


#include "ParentTankTurret.h"
#include "Components/ArrowComponent.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
AParentTankTurret::AParentTankTurret()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}


// Called when the game starts or when spawned
void AParentTankTurret::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters Params;
	Params.Owner = this;

	Cannon = GetWorld()->SpawnActor<ACannon>(CannonClass, Params);
	Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	
}
