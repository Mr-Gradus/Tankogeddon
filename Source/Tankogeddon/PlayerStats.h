#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "PlayerStats.generated.h"



UCLASS()
class TANKOGEDDON_API UPlayerStats : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UProgressBar* PlayerHealth;

	UPROPERTY(meta = (BindWidgetAnimOptional), Transient, BlueprintReadOnly)
	UWidgetAnimation* PlayerDamageAnim;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UTextBlock* PlayerCannon;

	UPROPERTY(meta = (BindWidgetOptional), BlueprintReadOnly)
	UTextBlock* PlayerAmmo;

public:
	virtual void NativeConstruct() override;

	void ChangeHealth(float CurrentHealth, float MaxHealth, bool bAnimation = true);

	void ChangeCannon(const FString& CannonClass);

	void ChangeAmmo(float CurrentAmmo, float MaxAmmo);

};
