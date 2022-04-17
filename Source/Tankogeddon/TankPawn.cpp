// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ATankPawn::ATankPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<UBoxComponent>("Collision");
	RootComponent = Collision;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank body"));
	BodyMesh->SetupAttachment(RootComponent);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank turret"));
	TurretMesh->SetupAttachment(BodyMesh);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring arm"));
	SpringArm->SetupAttachment(BodyMesh);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

void ATankPawn::MoveForward(float AxisValue)
{
	TargetForwardAxisValue = AxisValue;
}

void ATankPawn::RotateRight(float AxisValue)
{
	TargetRightAxisValue = AxisValue;
}


// Called when the game starts or when spawned
void ATankPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto Location = GetActorLocation();
	auto ForwardVector = GetActorForwardVector();
	SetActorLocation(Location + ForwardVector * TargetForwardAxisValue * MoveSpeed * DeltaTime);

	CurrentRightAxisValue = FMath::Lerp(CurrentRightAxisValue,
	TargetRightAxisValue, InterpolationKey);

	auto yawRotation = RotationSpeed * CurrentRightAxisValue * DeltaTime;
	FRotator currentRotation = GetActorRotation();

	yawRotation = currentRotation.Yaw + yawRotation;

	FRotator newRotation = FRotator(0, yawRotation, 0);

	SetActorRotation(newRotation);
	

	/*auto Rotation = GetActorRotation();
	Rotation.Yaw = Rotation.Yaw * RotationSpeed * TargetRightAxisValue * DeltaTime;
	SetActorRotation(Rotation * RotationSpeed * TargetRightAxisValue * DeltaTime);*/
	
	
	


	/*FVector currentLocation = GetActorLocation();

	FVector forwardVector = GetActorForwardVector();

	FVector rightVector = GetActorRightVector();
	
	FVector movePosition = currentLocation + ((forwardVector * MoveSpeed * TargetForwardAxisValue) + (rightVector * RotationSpeed * TargetRightAxisValue)) * DeltaTime;

//	FVector rmovePosition = currentLocation + rightVector * RotationSpeed * TargetRightAxisValue * DeltaTime;
	
	SetActorLocation(movePosition, true);
	//SetActorLocation(rmovePosition, true);
*/
}

// Called to bind functionality to input
void ATankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

