// Fill out your copyright notice in the Description page of Project Settings.


#include "ParentTankTurret.h"
#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"


void AParentTankTurret::OnDeath()
{
}

void AParentTankTurret::OnHealthChanged(float Health)
{
}

// Sets default values
AParentTankTurret::AParentTankTurret()
{
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health Component");
	HealthComponent->OnDeath.AddUObject(this, &AParentTankTurret::OnDeath);
	HealthComponent->OnHealthChanged.AddUObject(this, &AParentTankTurret::OnHealthChanged);

	TargetRange = CreateDefaultSubobject<USphereComponent>("Target Range");
	TargetRange->SetupAttachment(RootComponent);
	TargetRange->OnComponentBeginOverlap.AddDynamic(this, &AParentTankTurret::OnTargetBeginOverlap);
	TargetRange->OnComponentEndOverlap.AddDynamic(this, &AParentTankTurret::OnTargetEndOverlap);
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

void AParentTankTurret::OnTargetBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Other == this)
		return;
	Targets.Add(Other);
	if (!BestTarget.IsValid())
	{
		FindBestTarget();
	}
}

void AParentTankTurret::OnTargetEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, 	int32 OtherBodyIndex)
{
	if (Other == this)
		return;
	Targets.Remove(Other);
	if (Other == BestTarget)
	{
		FindBestTarget();
	}
}