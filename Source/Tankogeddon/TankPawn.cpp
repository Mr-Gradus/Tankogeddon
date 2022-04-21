// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "TankPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ArrowComponent.h"


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

	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon setup point"));
    CannonSetupPoint->AttachToComponent(TurretMesh, FAttachmentTransformRules::KeepRelativeTransform);


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
	TankController = Cast<ATankPlayerController>(GetController());
	SetupCannon();
}



void ATankPawn::SetupCannon()
{
	if(Cannon)
	{
		Cannon->Destroy();
	}
	
	FActorSpawnParameters params;
	params.Instigator = this;
	params.Owner = this;

	Cannon = GetWorld()->SpawnActor<ACannon>(CannonClass, params);
	Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}


void ATankPawn::Destroyed()
{
	
if (Cannon)
	Cannon->Destroy();
}

void ATankPawn::Fire()
{
	if(Cannon)
	{
	Cannon->Fire();
	}
}


// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	CurrentForwardAxisValue = FMath::Lerp(CurrentForwardAxisValue, TargetForwardAxisValue, SpeedInterpolationKey);

	auto Location = GetActorLocation();
	auto ForwardVector = GetActorForwardVector();
	SetActorLocation(Location + ForwardVector * CurrentForwardAxisValue * MoveSpeed * DeltaTime, false);

	CurrentRightAxisValue = FMath::Lerp(CurrentRightAxisValue, TargetRightAxisValue, InterpolationKey);

	//UE_LOG(LogTanks, Warning, TEXT("CurrentRightAxisValue = %f TargetRightAxisValue 	= %f"), CurrentRightAxisValue, TargetRightAxisValue);
	
	auto yawRotation = RotationSpeed * CurrentRightAxisValue * DeltaTime;
	FRotator currentRotation = GetActorRotation();

	yawRotation = currentRotation.Yaw + yawRotation;
	FRotator newRotation = FRotator(0, yawRotation, 0);

	SetActorRotation(newRotation);

	//GEngine->AddOnScreenDebugMessage(-1, 0.1, FColor::Blue, FString::Printf(TEXT("TargetForwardAxisValue = %f"), CurrentForwardAxisValue), false);

	if(TankController)
	{

		auto MousePos = TankController->GetMousePos(); 
		auto TurretRotation = TurretMesh->GetComponentRotation(); 
		FRotator MouseRotation = UKismetMathLibrary::FindLookAtRotation(TurretMesh->GetComponentLocation(), MousePos);
		MouseRotation.Roll = TurretRotation.Roll; 
		MouseRotation.Pitch = TurretRotation.Pitch;  
		TurretMesh->SetWorldRotation(FMath::Lerp(TurretRotation, MouseRotation, TurretRotationInterpolationKey));

		//крутит башню только если камера сверху под прямым углом
		/*FVector mousePos = TankController->GetMousePos();
		FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), mousePos);
		FRotator currRotation = TurretMesh->GetComponentRotation();
		targetRotation.Pitch = currRotation.Pitch;
		targetRotation.Roll = currRotation.Roll;
		TurretMesh->SetWorldRotation(FMath::Lerp(currRotation, targetRotation, TurretRotationInterpolationKey));*/
	}
}

	/*auto Rotation = GetActorRotation();
	Rotation.Yaw = Rotation.Yaw * RotationSpeed * TargetRightAxisValue * DeltaTime;
	SetActorRotation(Rotation * RotationSpeed * TargetRightAxisValue * DeltaTime);*/
	
	/*FVector currentLocation = GetActorLocation();

	FVector forwardVector = GetActorForwardVector();

	FVector rightVector = GetActorRightVector();
	
	FVector movePosition = currentLocation + ((forwardVector * MoveSpeed * TargetForwardAxisValue) + (rightVector * RotationSpeed * TargetRightAxisValue)) * DeltaTime;

//	FVector rmovePosition = currentLocation + rightVector * RotationSpeed * TargetRightAxisValue * DeltaTime;
	
	SetActorLocation(movePosition, true);
	//SetActorLocation(rmovePosition, true);*/



// Called to bind functionality to input
void ATankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

