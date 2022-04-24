// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cannon.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Pawn.h"
#include "TankPawn.generated.h"

class UStaticMeshComponent;
class UCameraComponent;
class USpringArmComponent;
class ATankPlayerController;
class ACannon;

UCLASS()
class TANKOGEDDON_API ATankPawn : public APawn
{
	GENERATED_BODY()

protected:
		
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* BodyMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	USpringArmComponent * SpringArm;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UCameraComponent * Camera;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UArrowComponent * CannonSetupPoint;

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
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret|Cannon")
	TSubclassOf<ACannon> CannonClass;

	UPROPERTY()
	ACannon * Cannon;
	
	UPROPERTY()
	ATankPlayerController* TankController;

	float TargetForwardAxisValue = 0;
	float CurrentForwardAxisValue = 0;
	float TargetRightAxisValue = 0;
	float CurrentRightAxisValue = 0;
	float TargetTurretRightAxisValue = 0;
	TSubclassOf<ACannon> CannonClassSecond;

public:

	// Sets default values for this pawn's properties
	ATankPawn();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetupCannon(TSubclassOf<ACannon> NewCannonClass);


	UFUNCTION()
	void MoveForward(float AxisValue);

	UFUNCTION()
	void RotateRight(float AxisValue);

	UFUNCTION()
	void TurretRotateRight(float AxisValue);

	UFUNCTION()
	void Fire();

	UFUNCTION()
	void FireSpecial();

	UFUNCTION()
	void ChangeCannon();

	UFUNCTION()
	void IncreaseAmmo(int Ammo);


protected:
	UFUNCTION()
	void SetNewCannon(TSubclassOf<ACannon> InCannonClass);
	
	virtual void BeginPlay() override;
	void SetupCannon();
	void SetupCannon(void SetNewCannon(TSubclassOf<ACannon> InCannonClass));

	virtual void Destroyed() override;

	TSubclassOf<ACannon> CurrentCannon;

};