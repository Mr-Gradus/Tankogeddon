// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

class ATankPawn;

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATankPlayerController();
	
	virtual void SetupInputComponent() override;

	virtual void BeginPlay() override;

protected:
	UPROPERTY()
	ATankPawn* TankPawn;

	void MoveForward(float AxisValue);

	void RotateRight(float AxisValue);
	

	
	
	

	
};
