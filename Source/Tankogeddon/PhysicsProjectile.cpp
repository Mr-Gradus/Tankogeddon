// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsProjectile.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

APhysicsProjectile::APhysicsProjectile()
{
	MovementComponent = CreateDefaultSubobject<UPhysicsComponent>(TEXT("Movement Component"));	
}

void APhysicsProjectile::Start()
{
	Super::Start();

	MovementComponent->Velocity = GetActorForwardVector() * MoveSpeed;
	MovementComponent->SetComponentTickEnabled(true);
}

void APhysicsProjectile::Stop()
{
	MovementComponent->Velocity = FVector::ZeroVector;
	MovementComponent->SetComponentTickEnabled(false);

	Super::Stop();
}

void APhysicsProjectile::Tick(float DeltaSeconds)
{
	if (GetActorLocation().Z < 0)
	{
		Stop();
	}
}

void APhysicsProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other,	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Other == GetInstigator())
	{
		Stop();
		return;
	}
	
	if (bExplode)
	{
		Explode();

		FTransform Transform;
		Transform.SetLocation(GetActorLocation());
		Transform.SetScale3D(FVector(3,3,3));		
		
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, Transform);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitAudioEffect, GetActorLocation());
	}
	
	ExplodeDamage(SweepResult);
	
	Stop();
}





