// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameStructs.h"
#include "Projectile.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "Cannon.generated.h"

class UArrowComponent;
class UStaticMeshComponent;
class AProjectile;


UCLASS()
class TANKOGEDDON_API ACannon : public AActor
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UArrowComponent * ProjectileSpawnPoint;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Components")
	UAudioComponent* AudioComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Components")
	UParticleSystemComponent* VisualEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Effect")
	TSubclassOf<UCameraShakeBase> CameraShakeEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Effect")
	UForceFeedbackEffect* ForceFeedbackEffect;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float FireRate = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float FireRange = 1000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float FireDamage = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	ECannonType Type = ECannonType::FireProjectile;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret|Cannon")
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float FireSpecialRate = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float FireSpecialAmount = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int AmmoCount = 10;

	float Damage = 1.0f;

	FTimerHandle ReloadTimerHandle;
	FTimerHandle QueueTimerHandle;
	
	bool ReadyToFire = false;

	int CurrentQueue;

public:
	ACannon();

	bool IsReadyToFire() const;

	void Fire();
	
	void FireSpecial();	

	void Special();

	void SetAmmo(int SaveAmmo);

	int GetAmmo() const;

protected:
	virtual void BeginPlay() override;
	
	void Reload();

	
};
