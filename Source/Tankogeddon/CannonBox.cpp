// Fill out your copyright notice in the Description page of Project Settings.


#include "CannonBox.h"
#include "TankPawn.h"


// Sets default values
ACannonBox::ACannonBox()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent * SceeneCpm = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceeneCpm;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	Mesh->OnComponentBeginOverlap.AddDynamic(this, &ACannonBox::OnMeshOverlapBegin);
	Mesh->SetCollisionProfileName(FName("OverlapAll"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetGenerateOverlapEvents(true);
}

void ACannonBox::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATankPawn * PlayerPawn = Cast<ATankPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if(OtherActor == PlayerPawn)
	{
		PlayerPawn->SetupCannon(CannonClass);
		Destroy();
	}

}
