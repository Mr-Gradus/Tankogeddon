#pragma once

#include "CoreMinimal.h"
#include "RadioButtons.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Animation/WidgetAnimation.h"
#include "Components/ComboBoxString.h"
#include "MainMenuWidget.generated.h"

class UButton;
class URadioButtons;
class UComboBoxString;

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

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UButton* QuitBtn;

	//UPROPERTY(EditAnywhere, Transient, meta = (BindWidgetAnim))
	//UWidgetAnimation* MainMenuAnim;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	URadioButtons* RadioButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UComboBoxString* ThemeComboBox;
/*
	UPROPERTY(BlueprintReadOnly)
	FName Theme1 = "Standart";

	UPROPERTY(BlueprintReadOnly)
	FName Theme2 = "Black";

	UPROPERTY(BlueprintReadOnly)
	FName Theme3 = "Red";
*/	
public:

	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
protected:

	UFUNCTION()
	void OnNewGameClicked();

	UFUNCTION()
	void OnLoadClicked();

	UFUNCTION()
	void OnOptionClicked();

	UFUNCTION()
	void OnQuitClicked();

	UFUNCTION(BlueprintCallable)
	void SelectTheme();
};
