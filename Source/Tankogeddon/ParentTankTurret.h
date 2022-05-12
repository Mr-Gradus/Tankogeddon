// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Cannon.h"
#include "Components/BoxComponent.h"
#include "AmmoBox.h"
#include "DamageTaker.h"
#include "HealthComponent.h"
#include "TargetController.h"
#include "Kismet/KismetMathLibrary.h"
#include "ParentTankTurret.generated.h"

class UBoxComponent;
class UArrowComponent;
class UStaticMeshComponent;
class UHealthComponent;
class USphereComponent;
class ATankPlayerController;


UCLASS()
class TANKOGEDDON_API AParentTankTurret : public APawn, public IDamageTaker
{
	GENERATED_BODY()

	
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Components")
	UBoxComponent* Collision;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* BodyMesh;

	

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UArrowComponent* CannonSetupPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret|Cannon")
	TSubclassOf<ACannon> CannonClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret|Cannon")
	TSubclassOf<ACannon> SecondCannonClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	TSubclassOf<AAmmoBox> AmmoboxClass;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	USphereComponent* TargetRange;
	
	UPROPERTY()
	ACannon* Cannon;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float TargetSpeed = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float TargetRate = 0.005f;

	
public:
	virtual void OnDeath();
	virtual void OnHealthChanged(float Health);
	AParentTankTurret();

	virtual void BeginPlay() override;

	virtual void TakeDamage(const FDamageInfo& DamageInfo) override;

	UFUNCTION()
	void FindBestTarget();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* TurretMesh;
	
protected:

	class ITargetController* TargetController;

	TWeakObjectPtr<AActor> BestTarget;

	TSubclassOf<ACannon> CurrentCannon;

	UPROPERTY()
	TArray<AActor*> Targets;


};
