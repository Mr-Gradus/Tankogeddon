// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"

#include "TankPawn.h"

ATankPlayerController::ATankPlayerController()
{
}
void ATankPlayerController::SetupInputComponent()
{
Super::SetupInputComponent();
InputComponent->BindAxis("MoveForward", this,
&ATankPlayerController::MoveForward);

InputComponent->BindAxis("MoveRight", this,
&ATankPlayerController::MoveRight);
}
void ATankPlayerController::BeginPlay()
{
Super::BeginPlay();
TankPawn = Cast<ATankPawn>(GetPawn());
}
void ATankPlayerController::MoveForward(float AxisValue)
{
TankPawn->MoveForward(AxisValue);
}

void ATankPlayerController::MoveRight(float AxisValue)
{
	TankPawn->MoveRight(AxisValue);
}