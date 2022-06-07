// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

float UHealthComponent::GetHealth() const
{
	return CurrentHealth;
}

float UHealthComponent::GetHealthPercent() const
{
	return  CurrentHealth / MaxHealth;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;

}
void UHealthComponent::TakeDamage(const FDamageInfo& DamageInfo)
{
	const float PrevHealth = CurrentHealth;

	CurrentHealth -= DamageInfo.Damage;

	if (CurrentHealth != PrevHealth)
	{
		if (OnHealthChanged.IsBound())
			OnHealthChanged.Broadcast(CurrentHealth);
	}
	if (CurrentHealth <= 0)
	{
		CurrentHealth = 0;
		if (OnDeath.IsBound())
			OnDeath.Broadcast();
	}
	
}
void UHealthComponent::TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}


