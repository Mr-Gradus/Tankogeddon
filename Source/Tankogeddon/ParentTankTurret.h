#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tankogeddon/Armory/Cannon.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Tankogeddon/Armory/AmmoBox.h"
#include "Tankogeddon/Setting/DamageTaker.h"
#include "Tankogeddon/Setting/HealthComponent.h"
#include "Tankogeddon/Setting/TargetController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Setting/GameStructs.h"
#include "ParentTankTurret.generated.h"

class UBoxComponent;
class UArrowComponent;
class UStaticMeshComponent;
class UHealthComponent;
class USphereComponent;
class ATankPlayerController;
class UParticleSystem;
class USoundBase;

UCLASS()
class TANKOGEDDON_API AParentTankTurret : public APawn, public IDamageTaker
{
	GENERATED_BODY()

	DECLARE_EVENT(AParentTankTurret, PlayerDeathEvent);

	
public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Components")
	UBoxComponent* Collision;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* BodyMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* DestructObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	USoundBase* DestructSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	USoundBase* HitPlayerSound;

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

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UBoxComponent* HitCollider;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float TargetingRange = 1000;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float TargetingSpeed = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float TargetingRate = 0.005f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float Accurency = 10;

	UPROPERTY()
	APawn* PlayerPawn;

	UPROPERTY()
	ACannon* Cannon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UWidgetComponent* ProgressBarWidgetComponent;


public:
	AParentTankTurret();
	void PossessedBy(AController* NewController);

	virtual void Fire();

	void TakeDamage(FDamageInfo DamageInfo);

	virtual void Death();

	virtual void OnHealthChanged(float Health);

	virtual void BeginPlay() override;

	//virtual void TakeDamage(const FDamageInfo DamageInfo) override;

	FVector GetEyesPosition() const;

	void SetHealth();

	ITargetController* TargetController;

	TSubclassOf<ACannon> CurrentCannon;

	UPROPERTY()
	TArray<AActor*> Targets;

	PlayerDeathEvent PlayerDeath;

	UFUNCTION(BlueprintPure, Category = "Cannon")
	ACannon* GetCannon() const;

	UFUNCTION(BlueprintPure, Category = "Health")
	UHealthComponent* GetHealthComponent();
};
