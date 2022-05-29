
#include "Turret.h"
#include "TankPlayerController.h"
#include "Cannon.h"
#include "DrawDebugHelpers.h"
#include "TankPawn.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"


ATurret::ATurret()
{
	PrimaryActorTick.bCanEverTick = false;

	Collision = CreateDefaultSubobject<UBoxComponent>("Collision");
	RootComponent = Collision;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret body"));
	BodyMesh->SetupAttachment(RootComponent);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret turret"));
	TurretMesh->AttachToComponent(BodyMesh, FAttachmentTransformRules::KeepRelativeTransform);
	
	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon setup point"));
	CannonSetupPoint->AttachToComponent(TurretMesh, FAttachmentTransformRules::KeepRelativeTransform);

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
	HitCollider->SetupAttachment(TurretMesh);
	
	HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health Component");
	HealthComponent->OnDeath.AddUObject(this, &ATurret::Death);
	HealthComponent->OnHealthChanged.AddUObject(this, &ATurret::OnHealthChanged);
}

void ATurret::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UStaticMesh * TurretMeshTemp = LoadObject<UStaticMesh>(this, *TurretMeshPath);
	if(TurretMeshTemp)
		TurretMesh->SetStaticMesh(TurretMeshTemp);

	UStaticMesh * BodyMeshTemp = LoadObject<UStaticMesh>(this, *BodyMeshPath);
	if(BodyMeshTemp)
		BodyMesh->SetStaticMesh(BodyMeshTemp);
}

void ATurret::BeginPlay()
{
	AParentTankTurret::BeginPlay();

	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

	FTimerHandle TargetingTimerHandle;

	GetWorld()->GetTimerManager().SetTimer(TargetingTimerHandle, this, &ATurret::Targeting, TargetingRate, true, TargetingRate);

}

void ATurret::OnHealthChanged(const float Health)
{
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Cyan, FString::Printf(TEXT("Turret HP %f"), Health));
}

void AParentTankTurret::TakeDamage(const FDamageInfo DamageInfo)
{
	HealthComponent->TakeDamage(DamageInfo);
}

void ATurret::Destroyed()
{
	Super::Destroyed();

	if (Cannon)
		Cannon->Destroy();
}

void ATurret::Targeting()
{
	if (IsPlayerInRange())
	{
		RotateToPlayer();
	}

	if (CanFire() && Cannon && Cannon->IsReadyToFire())
	{
		Fire();
	}
}

void ATurret::RotateToPlayer() const
{
	const FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerPawn->GetActorLocation());
	FRotator CurrRotation = TurretMesh->GetComponentRotation();
	CurrRotation.Pitch = TargetRotation.Pitch;
	TurretMesh->SetWorldRotation(FMath::Lerp(CurrRotation, TargetRotation, TargetingSpeed));
}

bool ATurret::IsPlayerInRange() const
{
	return FVector::Distance(PlayerPawn->GetActorLocation(), GetActorLocation()) <= TargetingRange;
}

bool ATurret::CanFire() const
{
	if(!IsPlayerSeen())
		return false;

	const FVector TargetingDir = TurretMesh->GetForwardVector();
    FVector DirToPlayer = PlayerPawn->GetActorLocation() - GetActorLocation();
    DirToPlayer.Normalize();
	const float AimAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(TargetingDir, DirToPlayer)));
    return AimAngle <= Accurency;
}		
bool ATurret::DetectPlayerVisibility() const
{
	FVector PlayerPos = PlayerPawn->GetActorLocation();
	FVector EyesPos = this->GetEyesPosition();

	FHitResult HitResult;

	FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
	TraceParams.bTraceComplex = true;
	TraceParams.bReturnPhysicalMaterial = false;

	if (GetWorld()->LineTraceSingleByChannel(HitResult, EyesPos, PlayerPos, ECollisionChannel::ECC_Visibility, TraceParams))
	{
		if (HitResult.Actor.Get())
		{
			//DrawDebugLine(GetWorld(), EyesPos, HitResult.Location, FColor::Orange, false, 0.5f, 0, -1);
			return HitResult.Actor.Get() == PlayerPawn;
		}
	}
	//DrawDebugLine(GetWorld(), EyesPos, PlayerPos, FColor::Orange, false, 0.5f, 0, -1);
	return false;
}

bool ATurret::IsPlayerSeen() const
{
	FVector PlayerPos = PlayerPawn->GetActorLocation();
	FVector EyesPos = this->GetEyesPosition();

	FHitResult HitResult;

	FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
	TraceParams.bTraceComplex = true;
	TraceParams.AddIgnoredActor(this);
	TraceParams.bReturnPhysicalMaterial = false;

	if(GetWorld()->LineTraceSingleByChannel(HitResult, EyesPos, PlayerPos, ECollisionChannel::ECC_Visibility, TraceParams))
	{
		if(HitResult.Actor.Get())
		{
			//DrawDebugLine(GetWorld(), EyesPos, HitResult.Location, FColor::Orange, false, 0.5f, 0, -1);
			return HitResult.Actor.Get() == PlayerPawn;
		}
	}
	//DrawDebugLine(GetWorld(), EyesPos, PlayerPos, FColor::Orange, false, 0.5f, 0, -1);
	return false;
}
