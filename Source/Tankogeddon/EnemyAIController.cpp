// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "DrawDebugHelpers.h"
#include "TankPawn.h"
#include "Waypoint.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

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

	WaypointActors.Sort([](const AActor& a, const AActor& b)
	{
		auto WPA = CastChecked<AWaypoint>(&a);
		auto WPB = CastChecked<AWaypoint>(&b);
		return WPA->Order > WPB->Order;
	});

	for(auto Waypoint : WaypointActors)
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
		return;
	if (Waypoints.Num() <= 0)
		return;
	if (NextWaypoint >= Waypoints.Num())
		NextWaypoint = 0;
	auto Waypoint = Waypoints[NextWaypoint];

	if (2500 < FVector::DistSquared2D(Waypoint, TankPawn->GetActorLocation()))
	{
		TankPawn->MoveForward(1);
	}
	else
	{
		NextWaypoint++;
	}

	auto TargetRotation = UKismetMathLibrary::FindLookAtRotation(TankPawn->GetActorLocation(), Waypoint);
	//TargetRotation.Pitch = 0;
	//TargetRotation.Roll = 0;
	auto Rotation = TankPawn->GetActorRotation();
	//Rotation.Pitch = 0;
	//Rotation.Roll = 0;

	auto Direction = FRotator::NormalizeAxis(TargetRotation.Yaw - Rotation.Yaw);
		
	auto Diff = FMath::Abs(Direction);
	 
	if ( Diff >= 5)
	{
		if (Diff > 60)
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

void AEnemyAIController::RotateToPlayer()
{
	if(IsPlayerInRange())
	TankPawn->RotateTurretTo(PlayerPawn->GetActorLocation());
}
bool AEnemyAIController::IsPlayerInRange()
{
	return FVector::Distance(TankPawn->GetActorLocation(), PlayerPawn->GetActorLocation()) <= TargetingRange;
}
bool AEnemyAIController::CanFire()
{
	if(!IsPlayerSeen())
		return false;

	FVector targetingDir = TankPawn->GetTurretForwardVector();
	auto dirToPlayer = PlayerPawn->GetActorLocation() - TankPawn->GetActorLocation();
	dirToPlayer.Normalize();
	float aimAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(targetingDir,
	dirToPlayer)));
	return aimAngle <= Accurency;
}
void AEnemyAIController::Fire()
{
	TankPawn->Fire();
}

bool AEnemyAIController::IsPlayerSeen()
{
	FVector playerPos = PlayerPawn->GetActorLocation();
	FVector eyesPos = TankPawn->GetEyesPosition();
	FHitResult hitResult;
	FCollisionQueryParams traceParams =
	FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
	traceParams.bTraceComplex = true;
	traceParams.AddIgnoredActor(TankPawn);
	traceParams.bReturnPhysicalMaterial = false;
	if(GetWorld()->LineTraceSingleByChannel(hitResult, eyesPos, playerPos, ECollisionChannel::ECC_Visibility, traceParams))
	{
		if(hitResult.Actor.Get())
		{
		DrawDebugLine(GetWorld(), eyesPos, hitResult.Location, FColor::Blue, false, 0.5f, 0, 10);
		return hitResult.Actor.Get() == PlayerPawn;
		}
	}
	DrawDebugLine(GetWorld(), eyesPos, playerPos, FColor::Blue, false, 0.5f, 0, 10);
	return false;
}

