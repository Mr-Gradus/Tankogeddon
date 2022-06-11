#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"


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

protected:

	

	FOnRadioChoiceChanged OnRadioChoiceChanged;

	ECheckBoxState IsRadioButtonChecked(int32 RadioButtonID);

	void HandleRadioButtonStateChanged(ECheckBoxState NewRadioState, int32 RadioButtonID);

	TSharedRef<SWidget> CreateRadioButton(const FString& RadioText, int32 RadioButtonChoice);

	TAttribute<int32> CurrentChoice;

	TAttribute<int32> CountCheckBox;

	int32 CurrentCheckBoxCount;

	TSharedPtr<SVerticalBox> VerticalBoxMake;

	int32 RadioButtonIndex = 0;

};
