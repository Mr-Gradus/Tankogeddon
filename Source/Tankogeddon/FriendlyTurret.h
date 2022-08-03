// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ParentTankTurret.h"
#include "Chaos/CollisionResolution.h"
#include "FriendlyTurret.generated.h"

class ATankPawn;
class UStaticMeshComponent;
class UHealthComponent;
class USphereComponent;

UCLASS()
class TANKOGEDDON_API AFriendlyTurret : public AParentTankTurret
{
	GENERATED_BODY()
	
protected:

	UPROPERTY()
	ATankPawn* TankPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Components")
	USphereComponent* TargetRange;

	UPROPERTY()
	TArray<AActor*> EnemyTargets;
public:	

	AFriendlyTurret();

	FHitResult VisibileTarget();

	
	void TurretRotation();
	
	void FindBestTarget();

	virtual void Fire() override;
	
	void OnTargetBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp,
	                          int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	void OnTargetEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp,
	                        int32 OtherBodyIndex);

	TWeakObjectPtr<AActor> BestTarget;

	const AActor* GetBestTarget() const
	{	
		return BestTarget.Get();
	}

	FRotator GetTurretRotation() const
	{
		return  TurretMesh->GetComponentRotation();
	}
	
protected:

	virtual void BeginPlay() override;

	virtual void OnHealthChanged(float Health) override;

	void Destroy();

	virtual void Tick(float DeltaTime) override;

};