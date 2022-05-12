// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "TankPlayerController.h"
#include "HealthComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"


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

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health Component");
	HealthComponent->OnDeath.AddUObject(this, &ATankPawn::OnDeath);
	HealthComponent->OnHealthChanged.AddUObject(this, &ATankPawn::OnHealthChanged);

	TargetRange = CreateDefaultSubobject<USphereComponent>("Target Range");
	TargetRange->SetupAttachment(RootComponent);
	TargetRange->OnComponentBeginOverlap.AddDynamic(this, &ATankPawn::OnTargetBeginOverlap);
	TargetRange->OnComponentEndOverlap.AddDynamic(this, &ATankPawn::OnTargetEndOverlap);
		
	
}

void ATankPawn::MoveForward(float AxisValue)
{
	TargetForwardAxisValue = AxisValue;
}

void ATankPawn::RotateRight(float AxisValue)
{
	TargetRightAxisValue = AxisValue;
}

void ATankPawn::TurretRotateRight(float AxisValue)
{
	TargetTurretRightAxisValue = AxisValue;
}

void ATankPawn::IncreaseAmmo(int Ammo)
{
	Cannon->SetAmmo(Cannon->GetAmmo() + Ammo);
}

void ATankPawn::OnDeath()
{
	Destroy();

	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}

void ATankPawn::OnHealthChanged(float Health)
{
	GEngine->AddOnScreenDebugMessage(23423, 999999, FColor::Magenta, FString::Printf(TEXT("Tank HP %f"), Health));
}

void ATankPawn::BeginPlay()
{
	Super::BeginPlay();
	TargetController = Cast<ITargetController>(GetController());
	CurrentCannon = CannonClass;
}

void ATankPawn::ChangeCannon()
{
	if (CurrentCannon == CannonClass)
	{
		int32 current = Cannon->GetAmmo();
		SetupCannon(SecondCannonClass);
		CurrentCannon = SecondCannonClass;
		Cannon->SetAmmo(current);
	}
	else
	{
		int32 current = Cannon->GetAmmo();
		SetupCannon(CannonClass);
		CurrentCannon = CannonClass;
		Cannon->SetAmmo(current);
	}
}

void ATankPawn::SetupCannon(TSubclassOf<ACannon> SelectCannonClass)
{
	if (Cannon)
	{
		Cannon->Destroy();
		Cannon = nullptr;
	}

	FActorSpawnParameters Params;
	Params.Instigator = this;
	Params.Owner = this;
	Cannon = GetWorld()->SpawnActor<ACannon>(SelectCannonClass, Params);
	Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void ATankPawn::SetNewCannon(TSubclassOf<ACannon> SelectCannonClass)
{
	if (CurrentCannon == CannonClass)
	{
		CurrentCannon = SelectCannonClass;
		CannonClass = SelectCannonClass;
		SetupCannon(CannonClass);
	}
	else
	{
		CurrentCannon = SelectCannonClass;
		SecondCannonClass = SelectCannonClass;
		SetupCannon(SecondCannonClass);
	}
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

void ATankPawn::FireSpecial()
{
	if(Cannon)
	{
		Cannon->FireSpecial();
	}
}

void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	CurrentForwardAxisValue = FMath::Lerp(CurrentForwardAxisValue, TargetForwardAxisValue, SpeedInterpolationKey);

	auto Location = GetActorLocation();
	auto ForwardVector = GetActorForwardVector();
	SetActorLocation(Location + ForwardVector * CurrentForwardAxisValue * MoveSpeed * DeltaTime, true);

	CurrentRightAxisValue = FMath::Lerp(CurrentRightAxisValue, TargetRightAxisValue, InterpolationKey);

	//UE_LOG(LogTanks, Warning, TEXT("CurrentRightAxisValue = %f TargetRightAxisValue 	= %f"), CurrentRightAxisValue, TargetRightAxisValue);
	
	auto yawRotation = RotationSpeed * CurrentRightAxisValue * DeltaTime;
	FRotator currentRotation = GetActorRotation();

	yawRotation = currentRotation.Yaw + yawRotation;
	FRotator newRotation = FRotator(0, yawRotation, 0);

	SetActorRotation(newRotation);

	//GEngine->AddOnScreenDebugMessage(-1, 0.1, FColor::Blue, FString::Printf(TEXT("TargetForwardAxisValue = %f"), CurrentForwardAxisValue), false);

	if(TargetController)
	{
		auto MousePos = TargetController->GetTargetLocation(); 
		auto TurretRotation = TurretMesh->GetComponentRotation(); 
		FRotator MouseRotation = UKismetMathLibrary::FindLookAtRotation(TurretMesh->GetComponentLocation(), MousePos);
		MouseRotation.Roll = TurretRotation.Roll; 
		MouseRotation.Pitch = TurretRotation.Pitch;  
		TurretMesh->SetWorldRotation(FMath::Lerp(TurretRotation, MouseRotation, TurretRotationInterpolationKey));
	}
}

void ATankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATankPawn::OnTargetBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Other == this)
		return;
	Targets.Add(Other);
	if (!BestTarget.IsValid())
	{
		FindBestTarget();
	}
}

void ATankPawn::OnTargetEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, 	int32 OtherBodyIndex)
{
	if (Other == this)
		return;
	Targets.Remove(Other);
	if (Other == BestTarget)
	{
		FindBestTarget();
	}
}
