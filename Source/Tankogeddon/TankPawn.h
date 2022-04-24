// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cannon.h"
#include "Components/BoxComponent.h"
#include "ParentTankTurret.h"
#include "TankPawn.generated.h"


class UStaticMeshComponent;
class UCameraComponent;
class USpringArmComponent;
class ATankPlayerController;
class ACannon;

UCLASS()
class TANKOGEDDON_API ATankPawn : public AParentTankTurret
{
	GENERATED_BODY()

protected:
		
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	USpringArmComponent * SpringArm;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UCameraComponent * Camera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	float MoveSpeed = 500;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	float RotationSpeed = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	float InterpolationKey = 0.07f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	float SpeedInterpolationKey = 0.06f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret|Speed")
	float TurretRotationInterpolationKey = 0.02f;
	
	UPROPERTY()
	ATankPlayerController* TankController;

	float TargetForwardAxisValue = 0;
	float CurrentForwardAxisValue = 0;
	float TargetRightAxisValue = 0;
	float CurrentRightAxisValue = 0;
	float TargetTurretRightAxisValue = 0;

public:

	// Sets default values for this pawn's properties
	ATankPawn();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	TSubclassOf<ACannon> CurrentCannon;


	UFUNCTION()
	void MoveForward(float AxisValue);

	UFUNCTION()
	void RotateRight(float AxisValue);

	UFUNCTION()
	void TurretRotateRight(float AxisValue);

	UFUNCTION()
	void FireSpecial();

	UFUNCTION()
	void ChangeCannon();

	UFUNCTION()
	void SetupCannon(TSubclassOf<ACannon> NewCannonClass);

	UFUNCTION()
	void SetNewCannon(TSubclassOf<ACannon> SelectCannonClass);


	UFUNCTION()
	void IncreaseAmmo(int Ammo);


protected:
	
	
	virtual void BeginPlay() override;

	
	virtual void Destroyed() override;

};