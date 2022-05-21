// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Waypoint.generated.h"

UCLASS()
class TANKOGEDDON_API AWaypoint : public AActor
{
	GENERATED_BODY()
	
public:	
	AWaypoint();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	int Order;
	
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
