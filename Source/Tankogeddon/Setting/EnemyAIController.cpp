// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "DrawDebugHelpers.h"
#include "Tankogeddon/TankPawn.h"
#include "Waypoint.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	Initilize();
	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Waypoints.Empty();

	TankPawn = Cast<ATankPawn>(InPawn);
	if (!TankPawn)
	{
		return;
	}
	
	TArray<AActor*> WaypointActors;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AWaypoint::StaticClass(), TankPawn->WaypointTag, WaypointActors);

	WaypointActors.Sort([](const AActor& A, const AActor& B)
	{
		const auto WPA = CastChecked<AWaypoint>(&A);
		const auto WPB = CastChecked<AWaypoint>(&B);
		return WPA->Order > WPB->Order;
	});

	for(const auto Waypoint : WaypointActors)
	{
		Waypoints.Add(Waypoint->GetActorLocation());
	}

	for(auto i = 1; i < Waypoints.Num(); i++)
	{
		DrawDebugLine(GetWorld(), Waypoints[i - 1], Waypoints[i], FColor::Green, true);
	}
	if (Waypoints.Num() > 1)
	{
		DrawDebugLine(GetWorld(), Waypoints[Waypoints.Num() - 1], Waypoints[0], FColor::Green, true);
	}
	
	NextWaypoint = 0;
}

void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!TankPawn)
		Initilize();
	
	if(!TankPawn || Waypoints.Num() <= 0)
		return;
	
	if (NextWaypoint >= Waypoints.Num())
		NextWaypoint = 0;
	
	const auto Waypoint = Waypoints[NextWaypoint];

	if (2500 < FVector::DistSquared2D(Waypoint, TankPawn->GetActorLocation()))
	{
		TankPawn->MoveForward(1);
	}
	else
	{
		NextWaypoint++;
	}

	const auto TargetRotation = UKismetMathLibrary::FindLookAtRotation(TankPawn->GetActorLocation(), Waypoint);

	const auto Rotation = TankPawn->GetActorRotation();
	
	const auto Direction = FRotator::NormalizeAxis(TargetRotation.Yaw - Rotation.Yaw);

	const auto Diff = FMath::Abs(Direction);
	 
	if ( Diff >= 5)
	{
		if (Diff > 75)
		{
			TankPawn->MoveForward(0);
		}
		TankPawn->RotateRight(Direction > 0 ? 1 : -1);
	}
	else
	{
		TankPawn->RotateRight(0);
	}

	Targeting();
}

void AEnemyAIController::Targeting()
{
	if(CanFire())
	Fire();
	else
	RotateToPlayer();
}

void AEnemyAIController::RotateToPlayer() const
{
	if(IsPlayerInRange())
	TankPawn->RotateTurretTo(PlayerPawn->GetActorLocation());
}
bool AEnemyAIController::IsPlayerInRange() const
{
	return FVector::Distance(TankPawn->GetActorLocation(), PlayerPawn->GetActorLocation()) <= TargetingRange;
}
bool AEnemyAIController::CanFire()
{
	if(!IsPlayerSeen())
		return false;

	const FVector TargetingDir = TankPawn->GetTurretForwardVector();
	auto DirToPlayer = PlayerPawn->GetActorLocation() - TankPawn->GetActorLocation();
	DirToPlayer.Normalize();
	const float AimAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(TargetingDir,DirToPlayer)));
	return AimAngle <= Accurency;
}
void AEnemyAIController::Fire() const
{
	TankPawn->Fire();
}

bool AEnemyAIController::IsPlayerSeen()
{
	if(!PlayerPawn)
		Initilize();

	FVector PlayerPos = PlayerPawn->GetActorLocation();
	FVector EyesPos = TankPawn->GetEyesPosition();
	FHitResult HitResult;
	FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
	TraceParams.bTraceComplex = true;
	TraceParams.AddIgnoredActor(TankPawn);
	TraceParams.bReturnPhysicalMaterial = false;
	if(GetWorld()->LineTraceSingleByChannel(HitResult, EyesPos, PlayerPos, ECollisionChannel::ECC_Visibility, TraceParams))
	{
		if(HitResult.Actor.Get())
		{
		return HitResult.Actor.Get() == PlayerPawn;
		}
	}
	return false;
}

void AEnemyAIController::Initilize()
{
	Waypoints.Empty();

	TankPawn = Cast<ATankPawn>(GetPawn());
	if (!TankPawn)
	{
		return;
	}
	TArray<AActor*> WaypointActors;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AWaypoint::StaticClass(), TankPawn->WaypointTag, WaypointActors);

	WaypointActors.Sort([](const AActor& a, const AActor& b)
	{
		const auto WPA = CastChecked<AWaypoint>(&a);
		const auto WPB = CastChecked<AWaypoint>(&b);
		return WPA->Order > WPB->Order;
	});

	for(const auto Waypoint : WaypointActors)
	{
		Waypoints.Add(Waypoint->GetActorLocation());
	}

	for(auto i = 1; i < Waypoints.Num(); i++)
	{
		DrawDebugLine(GetWorld(), Waypoints[i - 1], Waypoints[i], FColor::Green, true);
	}
	if (Waypoints.Num() > 1)
	{
		DrawDebugLine(GetWorld(), Waypoints[Waypoints.Num() - 1], Waypoints[0], FColor::Green, true);
	}
	
	NextWaypoint = 0;
}