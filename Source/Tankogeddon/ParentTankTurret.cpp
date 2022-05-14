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

void AParentTankTurret::OnDeath()
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

void AParentTankTurret::Destroyed()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DestructObject, GetActorTransform());
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DestructSound, GetActorLocation());
	if (Cannon)
	{
		Cannon->Destroy();
	}
}
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

void AParentTankTurret::FindBestTarget()
{
	if (DetectPlayerVisibility())
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
}

bool AParentTankTurret::DetectPlayerVisibility()
{
	FVector PlayerPos = PlayerPawn->GetActorLocation();
	FVector EyesPos = this->GetEyesPosition();

	FHitResult HitResult;
	FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
	TraceParams.bTraceComplex = true;
	TraceParams.bReturnPhysicalMaterial = false;

	if (GetWorld()->LineTraceSingleByChannel(HitResult, EyesPos, PlayerPos, ECollisionChannel::ECC_Visibility, TraceParams))
	{
		if (HitResult.Actor.Get())
		{
			DrawDebugLine(GetWorld(), EyesPos, HitResult.Location, FColor::Cyan, false, 0.5f, 0, 10);
			return HitResult.Actor.Get() == PlayerPawn;
		}
	}
	DrawDebugLine(GetWorld(), EyesPos, PlayerPos, FColor::Cyan, false, 0.5f, 0, 10);
	return false;
}
FVector AParentTankTurret::GetEyesPosition()
{
	return CannonSetupPoint->GetComponentLocation();
}
