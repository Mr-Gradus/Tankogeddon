// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankPawn.h"
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

	//float GetRotationgValue();

	void Targeting();

	void RotateToPlayer();

	bool IsPlayerInRange();

	bool CanFire();

	void Fire();

	TArray<FVector> Waypoints;

	int NextWaypoint = 0;

	
	
	
	
};
