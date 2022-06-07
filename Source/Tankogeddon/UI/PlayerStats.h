#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "PlayerStats.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class TANKOGEDDON_API UPlayerStats : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UProgressBar* PlayerHealth;

	//UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	//UWidgetAnimation* PlayerDamageAnim;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UTextBlock* PlayerCannon;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UTextBlock* PlayerAmmo;

public:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

};
