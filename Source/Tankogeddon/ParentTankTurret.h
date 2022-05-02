// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Cannon.h"
#include "Components/BoxComponent.h"
#include "AmmoBox.h"
#include "DamageTaker.h"
#include "HealthComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "ParentTankTurret.generated.h"

class UBoxComponent;
class UArrowComponent;
class UStaticMeshComponent;
class UHealthComponent;


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
	UStaticMeshComponent* TurretMesh;

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

	UPROPERTY()
	ACannon* Cannon;

	
	
public:	

	AParentTankTurret();

	UFUNCTION()
	void Fire();

	virtual void TakeDamage(const FDamageInfo& DamageInfo) override;

	

protected:



	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
