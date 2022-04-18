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
}

void ATankPlayerController::Tick(float DeltaTime)
{
	//Super::Tick(DeltaTime);


	FVector ScreenMousePosition;
	FVector MouseDirection; 
	DeprojectMousePositionToWorld(ScreenMousePosition, MouseDirection); 
	auto Z = FMath::Abs(GetPawn()->GetActorLocation().Z - ScreenMousePosition.Z);   
	MousePos = ScreenMousePosition - Z * MouseDirection / MouseDirection.Z;

//крутит башню только если камера сверху под прямым углом
	/*FVector mouseDirection;
	DeprojectMousePositionToWorld(MousePos, mouseDirection);

	FVector pawnPos = TankPawn->GetActorLocation();
	MousePos.Z = pawnPos.Z;

	FVector dir = MousePos - pawnPos;
	dir.Normalize();

	MousePos = pawnPos + dir * 1000;
	DrawDebugLine(GetWorld(), pawnPos, MousePos, FColor::Green, false, 0.1f, 0.5);*/
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

void ATankPlayerController::RotateRight(float AxisValue)
{
	TankPawn->RotateRight(AxisValue);
}



