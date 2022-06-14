#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Widgets/SCompoundWidget.h"

class UButton;

DECLARE_DELEGATE_OneParam(FOnRadioChoiceChanged, int32);

class TANKOGEDDON_API SRadioButtonsList : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SRadioButtonsList)
	{}
		SLATE_EVENT(FOnRadioChoiceChanged, OnRadioChoiceChanged)

		SLATE_ATTRIBUTE(int32, CountCheckBox);

		SLATE_ATTRIBUTE(int32, CurrentChoice);


	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	virtual void Tick(const FGeometry& AllottedGeometry, double InCurrentTime, float InDeltaTime) override;

	TAttribute<int32> CountCheckBox;

	
protected:

	

	FOnRadioChoiceChanged OnRadioChoiceChanged;

	ECheckBoxState IsRadioButtonChecked(int32 RadioButtonID) const;

	void HandleRadioButtonStateChanged(ECheckBoxState NewRadioState, int32 RadioButtonID);

	void BuildButtons();

	TSharedRef<SWidget> CreateRadioButton(const FString& RadioText, int32 RadioButtonChoice);

	TAttribute<int32> CurrentChoice;

	int32 CurrentCheckBoxCount = 0;

	TSharedPtr<SVerticalBox> VerticalBoxMake;

	int32 RadioButtonIndex = 0;

};
