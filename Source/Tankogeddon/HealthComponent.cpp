// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}

float UHealthComponent::GetHealth() const
{
	return Health;
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

}
void UHealthComponent::TakeDamage(const FDamageInfo& DamageInfo)
{
	const float PrevHealth = Health;
	Health -= DamageInfo.Damage;
	if (Health != PrevHealth)
	{
		if (OnHealthChanged.IsBound())
			OnHealthChanged.Broadcast(Health);
	}
	if (Health <= 0)
	{
		Health = 0;
		if (OnDeath.IsBound())
			OnDeath.Broadcast();
	}
}
// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


