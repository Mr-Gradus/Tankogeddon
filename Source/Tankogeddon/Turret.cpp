// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "Components/SphereComponent.h"


// Sets default values
ATurret::ATurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<UBoxComponent>("Collision");
	RootComponent = Collision;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank body"));
	BodyMesh->SetupAttachment(RootComponent);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank turret"));
	TurretMesh->SetupAttachment(BodyMesh);
	
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


	TargetRange = CreateDefaultSubobject<USphereComponent>("Target Range");
	TargetRange->SetupAttachment(RootComponent);
	TargetRange->OnComponentBeginOverlap.AddDynamic(this, &ATurret::OnTargetBeginOverlap);
	TargetRange->OnComponentEndOverlap.AddDynamic(this, &ATurret::OnTargetEndOverlap);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health Component");
	HealthComponent->OnDeath.AddUObject(this, &ATurret::OnDeath);
	HealthComponent->OnHealthChanged.AddUObject(this, &ATurret::OnHealthChanged);

}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
	

	if (CannonClass)
	{
		auto Transform = CannonSetupPoint->GetComponentTransform();
		Cannon = GetWorld()->SpawnActor<ACannon>(CannonClass, Transform);
		Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}

	FTimerHandle _targetingTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(_targetingTimerHandle, this, &ATurret::FindBestTarget, TargetRate, true, TargetRate);
}

void ATurret::OnTargetBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Other == this)
		return;
	Targets.Add(Other);
	if (!BestTarget.IsValid())
	{
		FindBestTarget();
	}
}

void ATurret::OnTargetEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, 	int32 OtherBodyIndex)
{
	if (Other == this)
		return;
	Targets.Remove(Other);
	if (Other == BestTarget)
	{
		FindBestTarget();
	}
}

/*
void ATurret::OnDeath()
{
	Destroy();
}
*/
void ATurret::OnHealthChanged(float Health)
{
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Cyan, FString::Printf(TEXT("Turret HP %f"), Health));
}



void AParentTankTurret::TakeDamage(const FDamageInfo& DamageInfo)
{
	HealthComponent->TakeDamage(DamageInfo);
}

void ATurret::Destroyed()
{
	Super::Destroyed();

	if (Cannon)
		Cannon->Destroy();
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (BestTarget.IsValid())
	{
		auto TurretRotation = TurretMesh->GetComponentRotation(); 
		FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(TurretMesh->GetComponentLocation(), BestTarget->GetActorLocation());
		TargetRotation.Roll = TurretRotation.Roll; 
		//TargetRotation.Pitch = TurretRotation.Pitch;
		//TargetRotation.Yaw = TurretRotation.Yaw;
		
		TurretMesh->SetWorldRotation(FMath::Lerp(TurretRotation, TargetRotation, 0.1f));
		if (TurretRotation.Equals(TargetRotation, 5))
		{
			if (Cannon)
				Cannon->Fire();
		}
	}
}

