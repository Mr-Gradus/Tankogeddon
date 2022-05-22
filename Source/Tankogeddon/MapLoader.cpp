// Fill out your copyright notice in the Description page of Project Settings.


#include "MapLoader.h"
#include "Kismet/GameplayStatics.h"

AMapLoader::AMapLoader()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent * SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = SceneComp;

	GatesMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gates Mesh"));
	GatesMesh->SetupAttachment(SceneComp);

	ActivatedLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Activated lights"));
    ActivatedLight->SetupAttachment(SceneComp);
	
    DeactivatedLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Deactivated lights"));
    DeactivatedLight->SetupAttachment(SceneComp);
	
    TriggerCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger collider"));
    TriggerCollider->SetupAttachment(SceneComp);
    TriggerCollider->OnComponentBeginOverlap.AddDynamic(this, &AMapLoader::OnTriggerOverlapBegin);

}

void AMapLoader::BeginPlay()
{
	Super::BeginPlay();

	SetActiveLights();
}

void AMapLoader::SetIsActivated(const bool NewIsActivated)
{
	IsActivated = NewIsActivated;
	SetActiveLights();
}
void AMapLoader::SetActiveLights() const
{
	ActivatedLight->SetHiddenInGame(!IsActivated);
	DeactivatedLight->SetHiddenInGame(IsActivated);
}
void AMapLoader::OnTriggerOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto Pawn = Cast<APawn>(OtherActor);
	if (Pawn && !LoadLevelName.IsNone())
	{
		if(Pawn->GetController() == GetWorld()->GetFirstPlayerController())
		{
			UGameplayStatics::OpenLevel(GetWorld(), LoadLevelName);
		}
	}
	
/*
	if(!IsActivated)
		return;

	APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if(OtherActor == PlayerPawn)
	{
		UGameplayStatics::OpenLevel(GetWorld(), LoadLevelName);
	}
	*/
}


