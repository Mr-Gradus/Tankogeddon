// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ParentTankTurret.h"
#include "Chaos/CollisionResolution.h"
#include "Cannon.h"
#include "Turret.generated.h"

class ATankPawn;

UCLASS()
class TANKOGEDDON_API ATurret : public AParentTankTurret
{
	GENERATED_BODY()
	
protected:	

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UBoxComponent * HitCollider;

	UPROPERTY()
	APawn* PlayerPawn;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float Accurency = 10;


	const FString BodyMeshPath = "StaticMesh'/Game/CSC/Meshes/SM_CSC_Tower1.SM_CSC_Tower1'";
	const FString TurretMeshPath = "StaticMesh'/Game/CSC/Meshes/SM_CSC_Gun1.SM_CSC_Gun1'";
/*
	UFUNCTION()
	void OnTargetBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnTargetEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
*/	
public:	
	ATurret();

protected:

	UFUNCTION()
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnDeath() override;

	UFUNCTION()
	virtual void OnHealthChanged(float Health) override;

	

	UFUNCTION()
	virtual void Destroyed() override;
	
	

	UFUNCTION()
	virtual void Tick(float DeltaTime) override;
};