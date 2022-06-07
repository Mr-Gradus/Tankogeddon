#include "SRadioButtonsList.h"
#include "SlateOptMacros.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Text/STextBlock.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SRadioButtonsList::Construct(const FArguments& InArgs)
{
	OnRadioChoiceChanged = InArgs._OnRadioChoiceChanged;

	CurrentChoice = ERadioChoice::Radio0;
	
	ChildSlot
	[
		SNew(SVerticalBox)

		+SVerticalBox::Slot()
		[
			CreateRadioButton("Option1", ERadioChoice::Radio0)
		]

		+SVerticalBox::Slot()
		[
			CreateRadioButton("Option2", ERadioChoice::Radio1)
		]

		+SVerticalBox::Slot()
		[
			CreateRadioButton("Option3", ERadioChoice::Radio2)

		]
	];
	
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

ECheckBoxState SRadioButtonsList::IsRadioButtonChecked(ERadioChoice RadioButtonID)
{
	return (CurrentChoice == RadioButtonID) ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

void SRadioButtonsList::HandleRadioButtonStateChanged(ECheckBoxState NewRadioState, ERadioChoice RadioButtonID)
{
	if (NewRadioState == ECheckBoxState::Checked)
	{
		CurrentChoice = RadioButtonID;
		OnRadioChoiceChanged.ExecuteIfBound(CurrentChoice);
	}
}

TSharedRef<SWidget> SRadioButtonsList::CreateRadioButton(const FString& RadioText, ERadioChoice RadioButtonChoice)
{
	return SNew(SCheckBox)

	.IsChecked(MakeAttributeRaw(this, &SRadioButtonsList::IsRadioButtonChecked, RadioButtonChoice))
	.OnCheckStateChanged(this, &SRadioButtonsList::HandleRadioButtonStateChanged, RadioButtonChoice)
	[
		SNew(STextBlock)
		.Text(FText::FromString(RadioText))
	];
}

