#pragma once

#include "CoreMinimal.h"
#include "Tankogeddon/Armory/Cannon.h"
#include "ParentTankTurret.h"
#include "Components/WidgetComponent.h"
#include "TankPawn.generated.h"


class UCameraComponent;
class USpringArmComponent;
class ATankPlayerController;
class ACannon;
class UHealthComponent;
class USphereComponent;
class UPlayerStats;

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

	float TargetForwardAxisValue = 0;
	float CurrentForwardAxisValue = 0;
	float TargetRightAxisValue = 0;
	float CurrentRightAxisValue = 0;
	float TargetTurretRightAxisValue = 0;

public:

	ATankPawn();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	FName WaypointTag;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION()
	void MoveForward(float AxisValue);

	UFUNCTION()
	void RotateRight(float AxisValue);

	UFUNCTION()
	void TurretRotateRight(float AxisValue);

	UFUNCTION()
	void FireSpecial() const;

	UFUNCTION()
	void ChangeCannon();

	UFUNCTION()
	void SetupCannon(TSubclassOf<ACannon> NewCannonClass);

	UFUNCTION()
	void SetNewCannon(TSubclassOf<ACannon> SelectCannonClass);

	UFUNCTION()
	void IncreaseAmmo(int Ammo) const;

	UPROPERTY()
	AParentTankTurret* ParentTankTurret;
	
	FRotator GetTurretRotation() const
	{
		return TurretMesh->GetComponentRotation();
	}

	virtual void Fire() override;

	UFUNCTION()
	FVector GetTurretForwardVector() const;

	UFUNCTION()
	void RotateTurretTo(FVector TargetPosition) const;
	

protected:
	UFUNCTION()
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void Destroyed() override;
	
	virtual void Death() override;

	UFUNCTION()
	virtual void OnHealthChanged(float Health) override;

	


};