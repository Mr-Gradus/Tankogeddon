#include "TankPlayerController.h"
#include "Tankogeddon/UI/GameHUD.h"
#include "Tankogeddon/TankPawn.h"


ATankPlayerController::ATankPlayerController(const FObjectInitializer & Obj) : Super(Obj)
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;

}

void ATankPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (InputComponent)
	{
		InputComponent->BindAxis("MoveForward", this, &ATankPlayerController::MoveForward);
		InputComponent->BindAxis("RotateRight", this, &ATankPlayerController::RotateRight);
		InputComponent->BindAction("Shoot", IE_Pressed, this, &ATankPlayerController::Fire);
		InputComponent->BindAction("AltShoot", IE_Pressed, this, &ATankPlayerController::FireSpecial);
		InputComponent->BindAction("ChangeCannon", IE_Pressed, this, &ATankPlayerController::ChangeCannon);
		InputComponent->BindAxis("GamepadRotateRight", this, &ATankPlayerController::GRotateRight);
		
		InputComponent->BindKey(EKeys::LeftMouseButton, IE_Released, this, FInputActionHandlerSignature::TUObjectMethodDelegate<ATankPlayerController>::FMethodPtr(
			                        &ATankPlayerController::OnLeftMouseButtonUp));

	}
}

void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
if (!GetPawn())
{
	return;
}
	
	FVector ScreenMousePosition;
	FVector MouseDirection; 

	DeprojectMousePositionToWorld(ScreenMousePosition, MouseDirection);
	auto Z = FMath::Abs(GetPawn()->GetActorLocation().Z - ScreenMousePosition.Z);   
	
	MousePos = ScreenMousePosition - Z * MouseDirection / MouseDirection.Z;
	
}

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if(TankPawn)
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
		TankPawn->TurretRotateRight(AxisValue);
	
}

void ATankPlayerController::OnLeftMouseButtonUp() const
{
	if (OnMouseButtonUp.IsBound())
    {
		OnMouseButtonUp.Broadcast();
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


void ATankPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	TankPawn = Cast<ATankPawn>(GetPawn());
}
