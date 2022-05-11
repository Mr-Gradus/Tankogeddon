#pragma once

#include "CoreMinimal.h"
#include "Cannon.h"
#include "Components/BoxComponent.h"
#include "DamageTaker.h"
#include "ParentTankTurret.h"
#include "TankPawn.generated.h"


class UCameraComponent;
class USpringArmComponent;
class ATankPlayerController;
class ACannon;
class UHealthComponent;

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
	void Fire();
	
	UFUNCTION()
	void FireSpecial();

	UFUNCTION()
	void ChangeCannon();

	UFUNCTION()
	void SetupCannon(TSubclassOf<ACannon> NewCannonClass);

	UFUNCTION()
	void SetNewCannon(TSubclassOf<ACannon> SelectCannonClass);

	UFUNCTION()
	virtual void BeginPlay() override;


	UFUNCTION()
	virtual void Destroyed() override;

	UFUNCTION()
	void IncreaseAmmo(int Ammo);

	UFUNCTION()
	virtual void OnDeath() override;

	UFUNCTION()
	virtual void OnHealthChanged(float Health) override;
/*
	UFUNCTION()
	void OnTargetBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnTargetEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
*/
};