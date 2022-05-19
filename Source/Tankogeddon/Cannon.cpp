// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include "DamageTaker.h"
#include "DrawDebugHelpers.h"
#include "Projectile.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Components/AudioComponent.h"
#include "Engine/Engine.h"
#include "Particles/ParticleSystemComponent.h"

ACannon::ACannon()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent * sceeneCpm = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = sceeneCpm;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon mesh"));
	Mesh->SetupAttachment(RootComponent);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawn point"));
	ProjectileSpawnPoint->SetupAttachment(Mesh);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>("AudioComponent");
	AudioComponent->SetupAttachment(RootComponent);

	VisualEffect = CreateDefaultSubobject<UParticleSystemComponent>("VisualEffect");
	VisualEffect->SetupAttachment(RootComponent);
}


void ACannon::Fire()
{
	if (AmmoCount == 0)
	{
		GEngine->AddOnScreenDebugMessage(10, 1, FColor::Red, "Need reload");
		return;
	}

	if(!ReadyToFire)
	{
		return;	
	}
	ReadyToFire = false;

	if (Type == ECannonType::FireProjectile)
	{

		GEngine->AddOnScreenDebugMessage(-1, 1,FColor::Black, "Fire - projectile");

		//FTransform projectileTransform(ProjectileSpawnPoint->GetComponentRotation(),
		//ProjectileSpawnPoint->GetComponentLocation(), FVector(1));

		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
		if (Projectile)
		{
			Projectile->SetInstigator(GetInstigator());
			Projectile->Start();

			AudioComponent->Play();
			VisualEffect->ActivateSystem(true);
			if (GetOwner() == GetWorld()->GetFirstPlayerController()->GetPawn())
			{
				if (CameraShakeEffect)
				{
					GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(CameraShakeEffect);
				}

				if (ForceFeedbackEffect)
				{
					FForceFeedbackParameters Params;
					Params.bLooping = false;
					GetWorld()->GetFirstPlayerController()->ClientPlayForceFeedback(ForceFeedbackEffect, Params);
				}
			}
		}
	}
			
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1,FColor::Red, "Fire - trace");

		FHitResult hitResult;
		FCollisionQueryParams traceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);

		traceParams.bTraceComplex = true;
		traceParams.bReturnPhysicalMaterial = false;
		
		FVector start = ProjectileSpawnPoint->GetComponentLocation();
		FVector end = ProjectileSpawnPoint->GetForwardVector() * FireRange + start;

		if (GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_Visibility, traceParams))
		{
			DrawDebugLine(GetWorld(), start, hitResult.Location, FColor::Red, false, 0.5f, 0, 5);

			AActor* MyInstigator = GetInstigator();

			if(hitResult.Actor.Get() != MyInstigator)
			{
				auto DamageTaker = Cast<IDamageTaker>(hitResult.Actor);
				if (DamageTaker)
				{
					FDamageInfo DamageInfo;
					DamageInfo.Damage = Damage;
					DamageInfo.DamageMaker = this;
					DamageInfo.Instigator = GetInstigator();
					DamageTaker->TakeDamage(DamageInfo);
				}
			}
		}
		else
		{
			DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 0.5f, 0, 5);
		}
	}

	AmmoCount--;
	GEngine->AddOnScreenDebugMessage(-1, 1,FColor::Blue, "Ammo" + FString::FromInt(AmmoCount));

	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1 / FireRate, false);
}


void ACannon::FireSpecial()
{
	if (AmmoCount == 0)
	{
		GEngine->AddOnScreenDebugMessage(10, 1, FColor::Red, "Need reload");
		return;
	}
	if(!ReadyToFire)
	{
		return;
	}
	ReadyToFire = false;

	CurrentQueue = FireSpecialAmount;

	GetWorldTimerManager().SetTimer(QueueTimerHandle, this, &ACannon::Special, 1 / FireSpecialRate, true);

	AmmoCount--;
	GEngine->AddOnScreenDebugMessage(-1, 1,FColor::Blue, "Ammo" + FString::FromInt(AmmoCount));

	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1 / FireRate, false);
}

	void ACannon::Special()
	{
		if (--CurrentQueue <= 0)
		{
			GetWorldTimerManager().ClearTimer(QueueTimerHandle);
		}
	if(Type == ECannonType::FireProjectile)
	{
		GEngine->AddOnScreenDebugMessage(10, 1,FColor::Yellow, "Fire - Special");

		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
		if (Projectile)
		{
			Projectile->Start();
		}

		FTransform projectileTransform(ProjectileSpawnPoint->GetComponentRotation(),
		ProjectileSpawnPoint->GetComponentLocation(), FVector(1));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1,FColor::Red, "Fire - trace");

		FHitResult hitResult;
		FCollisionQueryParams traceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);

		traceParams.bTraceComplex = true;
		traceParams.bReturnPhysicalMaterial = false;
		
		FVector start = ProjectileSpawnPoint->GetComponentLocation();
		FVector end = ProjectileSpawnPoint->GetForwardVector() * FireRange + start;

		if(GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_Visibility, traceParams))
		{
			DrawDebugLine(GetWorld(), start, hitResult.Location, FColor::Red, false, 0.5f, 0, 5);

			AActor* MyInstigator = GetInstigator();
			auto DamageTaker = Cast<IDamageTaker>(hitResult.Actor);
			if (DamageTaker)
			{
				FDamageInfo DamageInfo;
				DamageInfo.Damage = Damage;
				DamageInfo.DamageMaker = this;
				DamageInfo.Instigator = GetInstigator();
				DamageTaker->TakeDamage(DamageInfo);
			}
		}
		else
		{
			DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 0.5f, 0, 5);
		}
	}
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

int ACannon::GetAmmo()
{
	return AmmoCount;
}
void ACannon::SetAmmo(int SaveAmmo)
{
	AmmoCount = SaveAmmo;
}