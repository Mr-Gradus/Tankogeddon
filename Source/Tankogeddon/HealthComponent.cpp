// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

float UHealthComponent::GetHealth() const
{
	return Health;
}

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
void UHealthComponent::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}


