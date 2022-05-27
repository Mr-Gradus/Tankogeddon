// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ParentTankTurret.h"
#include "Chaos/CollisionResolution.h"
#include "Turret.generated.h"

class ATankPawn;
class UStaticMeshComponent;
class UHealthComponent;
class USphereComponent;

UCLASS()
class TANKOGEDDON_API ATurret : public AParentTankTurret
{
	GENERATED_BODY()
	
protected:
	
	const FString BodyMeshPath = "StaticMesh'/Game/CSC/Meshes/SM_CSC_Tower1.SM_CSC_Tower1'";
	const FString TurretMeshPath = "StaticMesh'/Game/CSC/Meshes/SM_CSC_Gun1.SM_CSC_Gun1'";

	UPROPERTY()
	ATankPawn* TankPawn;
	
public:	

	ATurret();
	
protected:

	virtual void BeginPlay() override;

	virtual void OnHealthChanged(float Health) override;

	virtual void Destroyed() override;

	void Targeting();

	void RotateToPlayer() const;

	bool IsPlayerInRange() const;

	bool DetectPlayerVisibility() const;

	bool IsPlayerSeen() const;

	bool CanFire() const;
};