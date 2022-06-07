// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include <Components/SceneComponent.h>
#include "TimerManager.h"
#include <Engine/World.h>
#include <Components/PrimitiveComponent.h>
#include "Tankogeddon/Setting/DamageTaker.h"
#include "Kismet/GameplayStatics.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	USceneComponent* sceeneCpm = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = sceeneCpm;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnMeshOverlapBegin);
	Mesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
}

void AProjectile::Start()
{
	GetWorld()->GetTimerManager().SetTimer(MovementTimerHandle, this, &AProjectile::Move, MoveRate, true, MoveRate);
	SetLifeSpan(FlyRange / MoveSpeed);
}

void AProjectile::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetInstigator())
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Projectile %s collided with %s. "), *GetName(), *OtherActor->GetName());
	if (OtherComp && OtherComp->GetCollisionObjectType() == ECollisionChannel::ECC_Destructible)
	{
		OtherActor->Destroy();
	}
	else if (IDamageTaker* DamageTaker = Cast<IDamageTaker>(OtherActor))
	{
        FDamageInfo DamageInfo;
        DamageInfo.Damage = Damage;
        DamageInfo.DamageMaker = this;
        DamageInfo.Instigator = GetInstigator();
		DamageTaker->TakeDamage(DamageInfo);
	}
	else
	{
		UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(OtherComp);
		if (PrimComp && PrimComp->IsSimulatingPhysics())
		{
			FVector ForceVector = GetActorForwardVector();
			PrimComp->AddImpulseAtLocation(ForceVector * PushForce, SweepResult.ImpactPoint);
		}
        if (bExplode)
        {
        	
        	Explode();
        }

	}
	Destroy();
}

void AProjectile::Move()
{
	FVector NextPosition = GetActorLocation() + GetActorForwardVector() * MoveSpeed * MoveRate;
	SetActorLocation(NextPosition);
}


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

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, EndPos);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitAudioEffect, GetActorLocation());
	
    bool SweepResult = GetWorld()->SweepMultiByChannel
    (
        AttackHit,
        StartPos,
        EndPos,
        Rotation,
        ECollisionChannel::ECC_Visibility,
        Shape,
        Params
    );

    GetWorld()->DebugDrawTraceTag = "Explode Trace";

    if (SweepResult)
    {
        for (FHitResult HitResult : AttackHit)
        {
            AActor* OtherActor = HitResult.GetActor();
            if (!OtherActor)
            {
                continue;
            }

        	
            ExplodeDamage(OtherActor);

        }
    }

}
void AProjectile::ExplodeDamage(AActor* OtherActor)
{
    IDamageTaker* DamageTakerActor = Cast<IDamageTaker>(OtherActor);
    if (DamageTakerActor)
    {
        FDamageInfo DamageInfo;
        DamageInfo.Damage = Damage;
        DamageInfo.Instigator = GetOwner();
        DamageInfo.DamageMaker = this;

        DamageTakerActor->TakeDamage(DamageInfo);
    }
    else
    {
        UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent());
        if (PrimComp)
        {
            if (PrimComp->IsSimulatingPhysics())
            {
                FVector ForceVector = OtherActor->GetActorLocation() - GetActorLocation();
                ForceVector.Normalize();
                PrimComp->AddImpulse(ForceVector * PushForce, NAME_None, true);
            }
        }
    }
}
