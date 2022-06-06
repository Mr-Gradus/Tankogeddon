// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HealthComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "HealthWidget.generated.h"

class UProgressBar;
/**
 * 
 */
UCLASS()
class TANKOGEDDON_API UHealthWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UProgressBar* TankPawnProgressBar;

public:
	virtual void NativeConstruct() override;

	void SetHealthValue(float Health) const;
};
