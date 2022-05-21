// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include <Components/SceneComponent.h>
#include "TimerManager.h"
#include <Engine/World.h>
#include "AITypes.h"
#include "DamageTaker.h"
#include "Tankogeddon.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent * SceeneCpm = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceeneCpm;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnMeshOverlapBegin);
	Mesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
}

void AProjectile::Start()
{
	GetWorld()->GetTimerManager().SetTimer(MovementTimerHandle, this, &AProjectile::Move, MoveRate, true, MoveRate);
}


void AProjectile::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
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

}

void AProjectile::Move()
{
	const FVector NextPosition = GetActorLocation() + GetActorForwardVector() * MoveRate * MoveSpeed;
	SetActorLocation(NextPosition);
}



