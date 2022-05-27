// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TANKOGEDDON_API UPhysicsComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UPhysicsComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gravity params")
	float Gravity = 980.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gravity params")
	FVector Velocity = FVector::ZeroVector;

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		
};
