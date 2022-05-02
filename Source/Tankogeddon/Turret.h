// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ParentTankTurret.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Chaos/CollisionResolution.h"
#include "Cannon.h"
#include "Turret.generated.h"

class ATankPawn;
class USphereComponent;
class UHealthComponent;

UCLASS()
class TANKOGEDDON_API ATurret : public AParentTankTurret
{
	GENERATED_BODY()
	
protected:	

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UBoxComponent * HitCollider;

	UPROPERTY()
	APawn* PlayerPawn;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	USphereComponent* TargetRange;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float TargetSpeed = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float TargetRate = 0.005f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float Accurency = 10;


	const FString BodyMeshPath = "StaticMesh'/Game/CSC/Meshes/SM_CSC_Tower1.SM_CSC_Tower1'";
	const FString TurretMeshPath = "StaticMesh'/Game/CSC/Meshes/SM_CSC_Gun1.SM_CSC_Gun1'";
	
public:	
	// Sets default values for this actor's properties
	ATurret();
	void BeginPlay();

protected:

	UFUNCTION()
	void OnTargetBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnTargetEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void FindBestTarget();
	
	TWeakObjectPtr<AActor> BestTarget;

	void OnDeath();

	void OnHealthChanged(float Health);


	UPROPERTY()
	TArray<AActor*> Targets;

	virtual void Destroyed() override;
	void Tick(float DeltaTime);
};