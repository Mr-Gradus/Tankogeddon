// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicsComponent.h"
#include "Projectile.h"
#include "PhysicsProjectile.generated.h"

class UPhysicsComponent;
class UParticleSystemComponent;

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API APhysicsProjectile : public AProjectile
{
	GENERATED_BODY()

public:
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UPhysicsComponent* MovementComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UParticleSystemComponent * TrailEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hit")
	UParticleSystem* HitEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hit")
	USoundBase* HitAudioEffect;

		
	APhysicsProjectile();

	virtual void Start() override;

	virtual void Stop() override;
	
	virtual void Tick(float DeltaSeconds) override;

	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
