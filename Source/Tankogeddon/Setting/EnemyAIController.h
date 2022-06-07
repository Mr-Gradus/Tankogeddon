// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Tankogeddon/TankPawn.h"
#include "EnemyAIController.generated.h"

class ATankPawn;
/**
 * 
 */
UCLASS()
class TANKOGEDDON_API AEnemyAIController : public AAIController  
{
	GENERATED_BODY()
	

protected:
	UPROPERTY()
	ATankPawn* TankPawn;

	UPROPERTY()
	APawn* PlayerPawn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float TargetingRange = 1000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float Accurency = 10;

	bool IsPlayerSeen();
	void Initilize();
public:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

	virtual void Tick(float DeltaTime) override;

	void Targeting();

	void RotateToPlayer() const;

	bool IsPlayerInRange() const;

	bool CanFire();

	void Fire() const;

	TArray<FVector> Waypoints;

	int NextWaypoint = 0;

	
	
	
	
};
