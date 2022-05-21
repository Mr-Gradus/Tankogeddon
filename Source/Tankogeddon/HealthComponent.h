// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameStructs.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TANKOGEDDON_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	//DECLARE_DELEGATE();
	DECLARE_EVENT(UHealthComponent, FDeathEvent);
	DECLARE_EVENT_OneParam(UHealthComponent, FHealthChangedEvent, float);
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Health")
	float MaxHealth = 10;

	UHealthComponent();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetHealth() const;

	void TakeDamage(const FDamageInfo& DamageInfo);
	
	FDeathEvent OnDeath;

	FHealthChangedEvent OnHealthChanged;
	
protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	float Health = 0;
		
};
