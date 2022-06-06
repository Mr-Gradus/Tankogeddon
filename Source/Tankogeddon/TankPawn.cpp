// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"

#include "GameHUD.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "TankPlayerController.h"
#include "HealthComponent.h"
#include "HealthWidget.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"


ATankPawn::ATankPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<UBoxComponent>("Collision");
	RootComponent = Collision;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank body"));
	BodyMesh->SetupAttachment(RootComponent);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank turret"));
	TurretMesh->SetupAttachment(BodyMesh);

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
	HitCollider->SetupAttachment(TurretMesh);

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
	HealthComponent->OnDeath.AddUObject(this, &ATankPawn::Death);
	HealthComponent->OnHealthChanged.AddUObject(this, &ATankPawn::OnHealthChanged);

	ProgressBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HP Bar"));
	ProgressBarWidgetComponent->SetupAttachment(BodyMesh);

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

FVector ATankPawn::GetTurretForwardVector() const
{
	return TurretMesh->GetForwardVector();
}

void ATankPawn::RotateTurretTo(FVector TargetPosition) const
{
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetPosition);
	const FRotator CurrRotation = TurretMesh->GetComponentRotation();
	TargetRotation.Pitch = CurrRotation.Pitch;
	TargetRotation.Roll = CurrRotation.Roll;
	TurretMesh->SetWorldRotation(FMath::Lerp(CurrRotation, TargetRotation, TurretRotationInterpolationKey));
}



void ATankPawn::IncreaseAmmo(const int Ammo) const
{
	Cannon->SetAmmo(Cannon->GetAmmo() + Ammo);
}

void ATankPawn::Death()
{
	if (IsPlayerControlled())
	{
		AGameHUD* HUD = Cast<AGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
		HUD->UseWidget(EWidgetID::GameOver);		
	}
	
	if (PlayerDeath.IsBound())
	{			
		PlayerDeath.Broadcast();					
	}

	FTransform Transform;
	Transform.SetLocation(GetActorLocation());

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DestructObject, GetActorTransform());
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DestructSound, GetActorLocation());

	
	if (Cannon)
	{
		Cannon->Destroy();
	}

	Destroy();
}

void ATankPawn::OnHealthChanged(const float Health)
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitPlayerSound, GetActorLocation());

	GEngine->AddOnScreenDebugMessage(23423, 999999, FColor::Magenta, FString::Printf(TEXT("Tank HP %f"), Health));

	
}



void ATankPawn::BeginPlay()
{
	Super::BeginPlay();
	TargetController = Cast<ITargetController>(GetController());
	CurrentCannon = CannonClass;

	AGameHUD* HUD = Cast<AGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	HUD->UseWidget(EWidgetID::PlayerStats);

	
}

void ATankPawn::ChangeCannon()
{
	if (CurrentCannon == CannonClass)
	{
		const int32 Current = Cannon->GetAmmo();
		SetupCannon(SecondCannonClass);
		CurrentCannon = SecondCannonClass;
		Cannon->SetAmmo(Current);
	}
	else
	{
		const int32 Current = Cannon->GetAmmo();
		SetupCannon(CannonClass);
		CurrentCannon = CannonClass;
		Cannon->SetAmmo(Current);
	}
}

void ATankPawn::SetupCannon(const TSubclassOf<ACannon> SelectCannonClass)
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

void ATankPawn::SetNewCannon(const TSubclassOf<ACannon> NewCannonClass)
{
	if (CurrentCannon == CannonClass)
	{
		CurrentCannon = NewCannonClass;
		CannonClass = NewCannonClass;
		SetupCannon(CannonClass);
	}
	else
	{
		CurrentCannon = NewCannonClass;
		SecondCannonClass = NewCannonClass;
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
	AParentTankTurret::Fire();
}

void ATankPawn::FireSpecial() const
{
	if (Cannon)
	{
		Cannon->FireSpecial();
	}
}

void ATankPawn::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Tank move
	CurrentForwardAxisValue = FMath::Lerp(CurrentForwardAxisValue, TargetForwardAxisValue, SpeedInterpolationKey);
	const auto Location = GetActorLocation();
	const auto ForwardVector = GetActorForwardVector();
	SetActorLocation(Location + ForwardVector * CurrentForwardAxisValue * MoveSpeed * DeltaTime, true);


	//Tank Rotation	
	CurrentRightAxisValue = FMath::Lerp(CurrentRightAxisValue, TargetRightAxisValue, InterpolationKey);
	auto YawRotation = RotationSpeed * CurrentRightAxisValue * DeltaTime;
	const FRotator CurrentRotation = GetActorRotation();
	YawRotation = CurrentRotation.Yaw + YawRotation;
	const FRotator NewRotation = FRotator(0, YawRotation, 0);
	SetActorRotation(NewRotation);

	if (TargetController)
	{
		const auto MousePos = TargetController->GetTargetLocation();
		const auto TurretRotation = TurretMesh->GetComponentRotation();
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

