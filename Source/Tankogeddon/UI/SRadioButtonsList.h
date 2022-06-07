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

	SLATE_END_ARGS()

	// Constructs this widget with InArgs
	void Construct(const FArguments& InArgs);

protected:
	ERadioChoice CurrentChoice;

	FOnRadioChoiceChanged OnRadioChoiceChanged;

	ECheckBoxState IsRadioButtonChecked(ERadioChoice RadioButtonID);

	void HandleRadioButtonStateChanged(ECheckBoxState NewRadioState, ERadioChoice RadioButtonID);

	TSharedRef<SWidget> CreateRadioButton(const FString& RadioText, ERadioChoice RadioButtonChoice);
	
};
