// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Animation/WidgetAnimation.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
	UButton* NewGameBtn;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* LoadBtn;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* OptionBtn;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* QuitBtn;

	//UPROPERTY(EditAnywhere, Transient, meta = (BindWidgetAnim))
	//UWidgetAnimation* MainMenuAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName LoadLevelName;
	
public:
	virtual void NativeConstruct() override;

	
protected:
	UFUNCTION()
	void OnNewGameClicked();

	UFUNCTION()
	void OnLoadClicked();

	UFUNCTION()
	void OnOptionClicked();

	UFUNCTION()
	void OnQuitClicked();

	
};
