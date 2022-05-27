// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include <Components/SceneComponent.h>
#include "TimerManager.h"
#include <Engine/World.h>
#include <Components/PrimitiveComponent.h>
#include "AITypes.h"
#include "DamageTaker.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickInterval = 0.005f;

	USceneComponent * SceeneCpm = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceeneCpm;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnMeshOverlapBegin);
	Mesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NextPosition = GetActorLocation() + GetActorForwardVector() * MoveSpeed * DeltaTime;
	SetActorLocation(NextPosition, true);

	if (FVector::Dist(GetActorLocation(), StartPosition) > MaxDistance)
	{
		Stop();
	}
}

void AProjectile::Start()
{
	PrimaryActorTick.SetTickFunctionEnable(true);
	
	StartPosition = GetActorLocation();
	
	Mesh->SetVisibility(true);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	SetLifeSpan(FlyRange / MoveSpeed);
}

void AProjectile::Stop()
{
	PrimaryActorTick.SetTickFunctionEnable(false);
	
	Mesh->SetVisibility(false);
	
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	Destroy();
	
}

void AProjectile::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetInstigator())
	{
		Stop();
		return;
	}

	if (bExplode)
	{
		Explode();		
	}
	else
	{
		ExplodeDamage(SweepResult);
	}
	
	Stop();
}
	
/*
	if (OtherActor == GetInstigator())
	{
		return;
	}

	UE_LOG(LogTanks, Warning, TEXT("Projectile %s collided with %s. "), *GetName(), *OtherActor->GetName());

	if (OtherComp && OtherComp->GetCollisionObjectType() == ECollisionChannel::ECC_Destructible)
	{
		OtherActor->Destroy();
	}
	
	if (OtherActor != GetInstigator() && OtherActor->GetInstigator() != GetInstigator())
	{
		const auto DamageTaker = Cast<IDamageTaker>(OtherActor);
		if (DamageTaker)
		{
			FDamageInfo DamageInfo;
			DamageInfo.Damage = Damage;
			DamageInfo.DamageMaker = this;
			DamageInfo.Instigator = GetInstigator();
			DamageTaker->TakeDamage(DamageInfo);
		}
		Destroy();
	}
	*/

void AProjectile::Explode()
{
	FVector StartPos = GetActorLocation();
	FVector EndPos = StartPos + FVector(0.1f);

	FCollisionShape Shape = FCollisionShape::MakeSphere(ExplodeRadius);
	FCollisionQueryParams Params = FCollisionQueryParams::DefaultQueryParam;
	Params.AddIgnoredActor(this);
	Params.bTraceComplex = true;
	Params.TraceTag = "Explode Trace";
	TArray<FHitResult> AttackHit;

	FQuat Rotation = FQuat::Identity;

	bool bSweepResult = GetWorld()->SweepMultiByChannel (AttackHit, StartPos, EndPos, Rotation, ECollisionChannel::ECC_Visibility, Shape, Params);

	if (bSweepResult)
	{
		for (FHitResult ExplosionHitResult : AttackHit)
		{
			AActor* HitActor = ExplosionHitResult.GetActor();
			if (!HitActor)
				continue;					
			
			ExplodeDamage(ExplosionHitResult);			
		}
	}
}
	
void AProjectile::ExplodeDamage(const FHitResult& HitResult)
{
	AActor* OtherActor = HitResult.GetActor();
	UPrimitiveComponent* OtherComp = Cast<UPrimitiveComponent>(HitResult.GetComponent());
	
	if (OtherActor && OtherComp && OtherComp->GetCollisionObjectType() == ECC_Destructible)
	{
		OtherActor->Destroy();
	}
	else if (IDamageTaker* Damageable = Cast<IDamageTaker>(OtherActor))
	{
		FDamageInfo DamageInfo;
		DamageInfo.Damage = Damage;
		DamageInfo.Instigator = GetInstigator();
		DamageInfo.DamageMaker = this;
		Damageable->TakeDamage(DamageInfo);
	}
	
	if (OtherComp && OtherComp->IsSimulatingPhysics())
	{
		FVector ForceVector = OtherActor->GetActorLocation() - GetActorLocation();
		ForceVector.Normalize();
		
		OtherComp->AddImpulseAtLocation(ForceVector * MoveSpeed, HitResult.ImpactPoint);		
	}	
}

void AProjectile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}
