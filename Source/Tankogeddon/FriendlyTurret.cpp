
#include "FriendlyTurret.h"
#include "Tankogeddon/Setting/TankPlayerController.h"
#include "Tankogeddon/Armory/Cannon.h"
#include "DrawDebugHelpers.h"
#include "TankPawn.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Setting/EnemyAIController.h"


AFriendlyTurret::AFriendlyTurret()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent * SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneComp;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret body"));
	BodyMesh->SetupAttachment(SceneComp);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret turret"));
	TurretMesh->AttachToComponent(BodyMesh, FAttachmentTransformRules::KeepRelativeTransform);
	
	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon setup point"));
	CannonSetupPoint->AttachToComponent(TurretMesh, FAttachmentTransformRules::KeepRelativeTransform);

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
	HitCollider->SetupAttachment(TurretMesh);
	
	HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health Component");
	HealthComponent->OnDeath.AddUObject(this, &AFriendlyTurret::Death);
	HealthComponent->OnHealthChanged.AddUObject(this, &AFriendlyTurret::OnHealthChanged);

	TargetRange = CreateDefaultSubobject<USphereComponent>("TargetRange");
	TargetRange->SetupAttachment(RootComponent);
	TargetRange->OnComponentBeginOverlap.AddDynamic(this, &AFriendlyTurret::OnTargetBeginOverlap);
	TargetRange->OnComponentEndOverlap.AddDynamic(this, &AFriendlyTurret::OnTargetEndOverlap);

	ProgressBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HP Bar"));
	ProgressBarWidgetComponent->SetupAttachment(BodyMesh);
}

void AFriendlyTurret::BeginPlay()
{
	Super::BeginPlay();
}

void AFriendlyTurret::OnHealthChanged(const float Health)
{
	Super::OnHealthChanged(Health);
}

void AFriendlyTurret::Destroy()
{
	Super::Destroy();
}

FHitResult AFriendlyTurret::VisibileTarget()
{
	FHitResult ActorVisible;
	FVector Start = GetActorLocation();
	FVector End = BestTarget->GetActorLocation();
		
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Vehicle);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Destructible);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetInstigator());
		
	GetWorld()->LineTraceSingleByObjectType(ActorVisible,Start,End,ObjectQueryParams,Params);	
	DrawDebugLine(GetWorld(),Start, ActorVisible.Location, FColor::Orange, false,2.5,0,10);

	return ActorVisible;
}

void AFriendlyTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	TurretRotation();	
}
void AFriendlyTurret::TurretRotation()
{
	if (BestTarget.IsValid() && VisibileTarget().Actor == BestTarget)
	{
		const FRotator TurretRotation = TurretMesh->GetComponentRotation();
		FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(TurretMesh->GetComponentLocation(), BestTarget->GetActorLocation());
		TargetRotation.Roll = TurretRotation.Roll;
		TargetRotation.Pitch = TurretRotation.Pitch;
		TurretMesh->SetWorldRotation(FMath::Lerp(TurretRotation, TargetRotation, 0.1f));

		if (TurretRotation.Equals(TargetRotation, 2))
		{			
			if (Cannon)
			{
				Cannon->Fire();
			}				

		}		
	}
}

void AFriendlyTurret::FindBestTarget()
{
	float MinDistance = 1000000;
	BestTarget = nullptr;
	for (AActor* Target:EnemyTargets)
	{
		float Distance = FVector::Dist2D(GetActorLocation(), Target->GetActorLocation());
		
		if (MinDistance > Distance)
		{						
			MinDistance = Distance;
			
			if (Target->ActorHasTag("Enemy"))
			{
				BestTarget = Target;	
			}														
		}
	}	
}

void AFriendlyTurret::Fire()
{
	if(Cannon)
	{
		Cannon->Fire();
	}
}
void AFriendlyTurret::OnTargetBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Other == this)
	{
		return;
	}
	
	EnemyTargets.Add(Other);
			
	if (!BestTarget.IsValid())
	{
		FindBestTarget();
	}
}

void AFriendlyTurret::OnTargetEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other,	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Other == this)
	{
		return;
	}
	
	EnemyTargets.Remove(Other);

	if (Other == BestTarget)
	{
		FindBestTarget();
	}
}