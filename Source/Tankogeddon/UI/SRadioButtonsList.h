#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"


UENUM(BlueprintType)
enum class ERadioChoice : uint8
{
	Radio0,
	Radio1,
	Radio2,
};

DECLARE_DELEGATE_OneParam(FOnRadioChoiceChanged, ERadioChoice /*NewRadioChoice*/);

class TANKOGEDDON_API SRadioButtonsList : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SRadioButtonsList)
	{}
		//Called when radio choice is changed 
		SLATE_EVENT(FOnRadioChoiceChanged, OnRadioChoiceChanged)

		SLATE_ATTRIBUTE(int32, CountCheckBox);

	SLATE_END_ARGS()

	// Constructs this widget with InArgs
	void Construct(const FArguments& InArgs);
	//virtual void Tick(const FGeometry& AllottedGeometry, double InCurrentTime, float InDeltaTime) override;

protected:

	//UPROPERTY(BlueprintReadWrite, Category = "RadioButtons")
	ERadioChoice CurrentChoice = ERadioChoice::Radio0;

	FOnRadioChoiceChanged OnRadioChoiceChanged;

	ECheckBoxState IsRadioButtonChecked(ERadioChoice RadioButtonID) const;

	void HandleRadioButtonStateChanged(ECheckBoxState NewRadioState, ERadioChoice RadioButtonID);

	TSharedRef<SWidget> CreateRadioButton(const FString& RadioText, ERadioChoice RadioButtonChoice);

	TAttribute<int32> CountCheckBox;

	TSharedPtr<SVerticalBox> VerticalBoxMake;
};
