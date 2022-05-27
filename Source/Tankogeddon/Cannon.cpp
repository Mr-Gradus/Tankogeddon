// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include "DamageTaker.h"
#include "DrawDebugHelpers.h"
#include "PhysicsComponent.h"
#include "PhysicsProjectile.h"
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

	USceneComponent * SceeneCpm = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceeneCpm;
	
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

	if (Type == ECannonType::FireProjectile)
	{
		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
		if (Projectile)
		{
			Projectile->SetInstigator(GetInstigator());
			Projectile->Start();
		}
	}
			
	else
	{
		FHitResult HitResult;
		FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);

		TraceParams.bTraceComplex = true;
		TraceParams.bReturnPhysicalMaterial = false;
		
		FVector Start = ProjectileSpawnPoint->GetComponentLocation();
		FVector End = ProjectileSpawnPoint->GetForwardVector() * FireRange + Start;

		if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility, TraceParams))
		{
			DrawDebugLine(GetWorld(), Start, HitResult.Location, FColor::Red, false, 0.5f, 0, 5);

			AActor* MyInstigator = GetInstigator();

			if(HitResult.Actor.Get() != MyInstigator)
			{
				auto DamageTaker = Cast<IDamageTaker>(HitResult.Actor);
				if (HitResult.Actor.Get() != MyInstigator)
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
			DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0.5f, 0, 5);
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

		//FTransform ProjectileTransform(ProjectileSpawnPoint->GetComponentRotation(), ProjectileSpawnPoint->GetComponentLocation(), FVector(1));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1,FColor::Red, "Fire - trace");

		FHitResult HitResult;
		FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);

		TraceParams.bTraceComplex = true;
		TraceParams.bReturnPhysicalMaterial = false;
		
		FVector Start = ProjectileSpawnPoint->GetComponentLocation();
		FVector End = ProjectileSpawnPoint->GetForwardVector() * FireRange + Start;

		if(GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility, TraceParams))
		{
			DrawDebugLine(GetWorld(), Start, HitResult.Location, FColor::Red, false, 0.5f, 0, 5);

			AActor* MyInstigator = GetInstigator();
			auto DamageTaker = Cast<IDamageTaker>(HitResult.Actor);
			if (HitResult.Actor.Get() != MyInstigator)
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
			DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0.5f, 0, 5);
		}
	}
}

bool ACannon::IsReadyToFire() const
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

int ACannon::GetAmmo() const
{
	return AmmoCount;
}

void ACannon::SetAmmo(const int SaveAmmo)
{
	AmmoCount = SaveAmmo;
}

bool ACannon::PhysicsAmmo() const
{
	return Type == ECannonType::FireProjectile && ProjectileClass && ProjectileClass->IsChildOf<APhysicsProjectile>();
}

FVector ACannon::GetCurrentPhysicsAmmoTarget(float FloorAbsoluteHeight) const
{
	if (!PhysicsAmmo())
	{
		return GetActorLocation();
	}

	APhysicsProjectile* DefaultProjectile = ProjectileClass->GetDefaultObject<APhysicsProjectile>();
	check(DefaultProjectile);

	float Angle = FMath::DegreesToRadians(ProjectileSpawnPoint->GetForwardVector().Rotation().Pitch);
	float Speed = DefaultProjectile->MoveSpeed;
	float Gravity = DefaultProjectile->MovementComponent->Gravity;
	if (FMath::IsNearlyZero(Gravity))
	{
		return GetActorLocation();
	}

	float Z = ProjectileSpawnPoint->GetComponentLocation().Z - FloorAbsoluteHeight;

	float SqrtComp = FMath::Sqrt(FMath::Square(Speed * FMath::Sin(Angle)) + 2 * Gravity * Z);
	float Range = Speed * FMath::Cos(Angle) * (Speed * FMath::Sin(Angle) + SqrtComp) / Gravity;
	FVector FlatDirection = ProjectileSpawnPoint->GetForwardVector();
	FlatDirection.Z = 0.f;
	FlatDirection.Normalize();
	FVector Result = ProjectileSpawnPoint->GetComponentLocation() + FlatDirection * Range;
	Result.Z = FloorAbsoluteHeight;
	return Result;
}

bool ACannon::SetDesiredPhysicsAmmoTarget(const FVector& InTarget)
{
	if (!PhysicsAmmo())
	{
		return false;
	}

	APhysicsProjectile* DefaultProjectile = ProjectileClass->GetDefaultObject<APhysicsProjectile>();
	check(DefaultProjectile);

	float Speed = DefaultProjectile->MoveSpeed;
	float Gravity = DefaultProjectile->MovementComponent->Gravity;
	if (FMath::IsNearlyZero(Gravity))
	{
		return false;
	}
	float Z = ProjectileSpawnPoint->GetComponentLocation().Z - InTarget.Z;
	float X = FVector::Dist2D(ProjectileSpawnPoint->GetComponentLocation(), InTarget);
	float Angle = 90.f; 
	if (!FMath::IsNearlyZero(X))
	{
		float FirstSqrtComp = FMath::Pow(Speed, 4);
		float SecondSqrtComp = Gravity * (Gravity * FMath::Square(X) - 2 * (FMath::Square(Speed) * Z));
		float SqrtComp = 0.f;
		if (FirstSqrtComp > SecondSqrtComp)
		{
			SqrtComp = FMath::Sqrt(FirstSqrtComp - SecondSqrtComp);
		}
		Angle = FMath::Atan((FMath::Square(Speed) + SqrtComp) / (Gravity * X));
		Angle = FMath::RadiansToDegrees(Angle);
	}
   
	FRotator DesiredRotation = GetActorRotation();
	DesiredRotation.Pitch = Angle;
	SetActorRotation(DesiredRotation);
	return true;
}