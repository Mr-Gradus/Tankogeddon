
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

	UStaticMesh * turretMeshTemp = LoadObject<UStaticMesh>(this, *TurretMeshPath);
	if(turretMeshTemp)
		TurretMesh->SetStaticMesh(turretMeshTemp);

	UStaticMesh * bodyMeshTemp = LoadObject<UStaticMesh>(this, *BodyMeshPath);
	if(bodyMeshTemp)
		BodyMesh->SetStaticMesh(bodyMeshTemp);


	HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health Component");
	HealthComponent->OnDeath.AddUObject(this, &ATurret::Death);
	HealthComponent->OnHealthChanged.AddUObject(this, &ATurret::OnHealthChanged);

}

void ATurret::BeginPlay()
{
	AParentTankTurret::BeginPlay();

	//FActorSpawnParameters params;
    //params.Owner = this;
    //Cannon = GetWorld()->SpawnActor<ACannon>(CannonClass, params);
    //Cannon->AttachToComponent(CannonSetupPoint,
   // FAttachmentTransformRules::SnapToTargetNotIncludingScale);
    PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
    FTimerHandle _targetingTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(_targetingTimerHandle, this, &ATurret::Targeting, TargetingRate, true, TargetingRate);

}

void ATurret::OnHealthChanged(float Health)
{
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Cyan, FString::Printf(TEXT("Turret HP %f"), Health));
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


void ATurret::RotateToPlayer()
{
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerPawn->GetActorLocation());
	FRotator CurrRotation = TurretMesh->GetComponentRotation();
	CurrRotation.Pitch = TargetRotation.Pitch;
	//CurrRotation.Roll = TargetRotation.Roll;
	//CurrRotation.Yaw = TargetRotation.Yaw; 
	TurretMesh->SetWorldRotation(FMath::Lerp(CurrRotation, TargetRotation, TargetingSpeed));
}

bool ATurret::IsPlayerInRange()
{
	return FVector::Distance(PlayerPawn->GetActorLocation(), GetActorLocation()) <= TargetingRange;
}


bool ATurret::CanFire()
{
	if(!IsPlayerSeen())
		return false;
	
	FVector TargetingDir = TurretMesh->GetForwardVector();
    FVector DirToPlayer = PlayerPawn->GetActorLocation() - GetActorLocation();
    DirToPlayer.Normalize();
    float AimAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(TargetingDir, DirToPlayer)));
    return AimAngle <= Accurency;
}		
bool ATurret::DetectPlayerVisibility()
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
			DrawDebugLine(GetWorld(), EyesPos, HitResult.Location, FColor::Orange, false, 0.5f, 0, 5);
			return HitResult.Actor.Get() == PlayerPawn;
		}
	}
	DrawDebugLine(GetWorld(), EyesPos, PlayerPos, FColor::Orange, false, 0.5f, 0, 5);
	return false;
}

bool ATurret::IsPlayerSeen()
{
	FVector playerPos = PlayerPawn->GetActorLocation();
	FVector eyesPos = this->GetEyesPosition();
	FHitResult hitResult;
	FCollisionQueryParams traceParams =
	FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
	traceParams.bTraceComplex = true;
	traceParams.AddIgnoredActor(this);
	traceParams.bReturnPhysicalMaterial = false;
	if(GetWorld()->LineTraceSingleByChannel(hitResult, eyesPos, playerPos, ECollisionChannel::ECC_Visibility, traceParams))
	{
		if(hitResult.Actor.Get())
		{
			DrawDebugLine(GetWorld(), eyesPos, hitResult.Location, FColor::Orange, false, 0.5f, 0, 10);
			return hitResult.Actor.Get() == PlayerPawn;
		}
	}
	DrawDebugLine(GetWorld(), eyesPos, playerPos, FColor::Orange, false, 0.5f, 0, 10);
	return false;
}
