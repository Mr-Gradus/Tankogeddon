// Fill out your copyright notice in the Description page of Project Settings.


#include "ParentTankTurret.h"
#include "Blueprint/UserWidget.h"
#include "DrawDebugHelpers.h"
#include "Components/WidgetComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "UI/HealthWidget.h"


void AParentTankTurret::Fire()
{
	if (Cannon)
	{
		Cannon->Fire();
	}
}

void AParentTankTurret::TakeDamage(const FDamageInfo DamageInfo)
{
	HealthComponent->TakeDamage(DamageInfo);

	SetHealth();
}

void AParentTankTurret::Death()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DestructObject, GetActorTransform());
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DestructSound, GetActorLocation());
	
	if (AmmoboxClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.bNoFail = true;
		GetWorld()->SpawnActor<AAmmoBox>(AmmoboxClass, GetActorTransform(), SpawnParams);
	}
	Destroy();
}

void AParentTankTurret::OnHealthChanged(float Health)
{
	GEngine->AddOnScreenDebugMessage(3, 2.0f, FColor::Purple, FString::Printf(TEXT("%s HP: %f "), *GetName(), Health));

}


AParentTankTurret::AParentTankTurret()
{
	PrimaryActorTick.bCanEverTick = true;


}


void AParentTankTurret::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	TargetController = Cast<ITargetController>(NewController);
}

PRAGMA_DISABLE_OPTIMIZATION
void AParentTankTurret::SetHealth()
{
	if(IsValid(ProgressBarWidgetComponent))
	{
		const UHealthWidget* HealthWidget = Cast<UHealthWidget>(ProgressBarWidgetComponent->GetUserWidgetObject());
		
		if (IsValid(HealthWidget))
		{
			HealthWidget->SetHealthValue(GetHealthComponent()->GetHealthPercent());
		}
	}
}
PRAGMA_ENABLE_OPTIMIZATION

void AParentTankTurret::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters Params;
	Params.Owner = this;

	Cannon = GetWorld()->SpawnActor<ACannon>(CannonClass, Params);
	Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	SetHealth();
}

FVector AParentTankTurret::GetEyesPosition() const
{
	return CannonSetupPoint->GetComponentLocation();
}


ACannon* AParentTankTurret::GetCannon() const
{
	return Cannon;
}

UHealthComponent* AParentTankTurret::GetHealthComponent()
{
	return HealthComponent;
}
