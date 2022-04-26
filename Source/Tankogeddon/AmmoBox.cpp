// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoBox.h"
#include "TankPawn.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

// Sets default values
AAmmoBox::AAmmoBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent * sceeneCpm = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = sceeneCpm;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AAmmoBox::OnMeshOverlapBegin);
	Mesh->SetCollisionProfileName(FName("OverlapAll"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetGenerateOverlapEvents(true);
}

void AAmmoBox::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATankPawn * PlayerPawn = Cast<ATankPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if(OtherActor == PlayerPawn)
	{
		PlayerPawn->SetupCannon(CannonClass);
			Destroy();
	}
}

