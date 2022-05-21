// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"
#include "DrawDebugHelpers.h"
#include "TankPawn.h"


ATankPlayerController::ATankPlayerController()
{
	bShowMouseCursor = true;
}

void ATankPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this, &ATankPlayerController::MoveForward);
	InputComponent->BindAxis("RotateRight", this, &ATankPlayerController::RotateRight);
	InputComponent->BindAction("Shoot", IE_Pressed, this, &ATankPlayerController::Fire);
	InputComponent->BindAction("AltShoot", IE_Pressed, this, &ATankPlayerController::FireSpecial);
	InputComponent->BindAction("ChangeCannon", IE_Pressed, this, &ATankPlayerController::ChangeCannon);
	InputComponent->BindAxis("GamepadRotateRight", this, &ATankPlayerController::GRotateRight);
	//InputComponent->BindAxis("GamepadRotateLeft", this, &ATankPlayerController::GRotateLeft);

}


void ATankPlayerController::Tick(float DeltaTime)
{
	FVector ScreenMousePosition;
	FVector MouseDirection; 

	DeprojectMousePositionToWorld(ScreenMousePosition, MouseDirection);
	const auto Z = FMath::Abs(GetPawn()->GetActorLocation().Z - ScreenMousePosition.Z);   
	
	MousePos = ScreenMousePosition - Z * MouseDirection / MouseDirection.Z;
	DrawDebugLine(GetWorld(), TankPawn->GetActorLocation(), MousePos, FColor::Green, false, 0.1f, 0.5);
}

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();
	TankPawn = Cast<ATankPawn>(GetPawn());
}

void ATankPlayerController::MoveForward(float AxisValue)
{
	if (TankPawn)
	TankPawn->MoveForward(AxisValue);
}

void ATankPlayerController::RotateRight(float AxisValue)
{
	if (TankPawn)
	TankPawn->RotateRight(AxisValue);
}

FVector ATankPlayerController::GetTargetLocation() const
{
	return MousePos; 
}

void ATankPlayerController::GRotateRight(float AxisValue)
{
	if (TankPawn)
	{
		TankPawn->TurretRotateRight(AxisValue);
	}
}

void ATankPlayerController::Fire()
{
	if (TankPawn)
		TankPawn->Fire();
}

void ATankPlayerController::FireSpecial()
{
	if (TankPawn)
		TankPawn->FireSpecial();
}

void ATankPlayerController::ChangeCannon()
{
	if (TankPawn)
	TankPawn->ChangeCannon();
}
