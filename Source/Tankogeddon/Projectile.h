// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Projectile.generated.h"

UCLASS()
class TANKOGEDDON_API AProjectile : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* Mesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	float FlyRange = 2000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	float MoveRate = 0.005f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category ="Projectile")
	float MaxDistance = 3000.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
	float Damage = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
	float PushForce = 1000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
	bool bExplode = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (EditCondition = "bExplode == true", EditConditionHides), Category = "Damage")
	float ExplodeRadius = 50.0f;

	FTimerHandle MovementTimerHandle;

	FVector StartPosition;

public:	
	AProjectile();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	float MoveSpeed = 100.0f;
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	virtual void Start();

	UFUNCTION()
	virtual void Stop();
	
	UFUNCTION()
	void Explode();

	UFUNCTION()
	void ExplodeDamage(const FHitResult& HitResult);	

	UFUNCTION()
	void OnMeshOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
};
