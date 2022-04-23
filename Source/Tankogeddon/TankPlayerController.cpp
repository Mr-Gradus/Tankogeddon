// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"
#include "DrawDebugHelpers.h"
#include "TankPawn.h"
#include "Engine/Engine.h"


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
	//InputComponent->BindAction("ChangeCannon", IE_Pressed, this, &ATankPlayerController::ChangeCannon);
	
	//InputComponent->BindAxis("TurretRotateRight", this, &ATankPlayerController::TurretRotateRight);

}


void ATankPlayerController::Tick(float DeltaTime)
{
	//Super::Tick(DeltaTime);


	FVector ScreenMousePosition;
	FVector MouseDirection; 

	DeprojectMousePositionToWorld(ScreenMousePosition, MouseDirection); 
	auto Z = FMath::Abs(GetPawn()->GetActorLocation().Z - ScreenMousePosition.Z);   
	
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
	TankPawn->MoveForward(AxisValue);
}

void ATankPlayerController::RotateRight(float AxisValue)
{
	TankPawn->RotateRight(AxisValue);
}

/*
void ATankPlayerController::TurretRotateRight(float AxisValue)
{
	TankPawn->TurretRotateRight(AxisValue);
	GEngine->AddOnScreenDebugMessage(5,1,FColor::Blue, FString::SanitizeFloat(AxisValue));
}
*/
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

