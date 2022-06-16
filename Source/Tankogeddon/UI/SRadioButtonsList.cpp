#include "SRadioButtonsList.h"
#include "SlateOptMacros.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Text/STextBlock.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SRadioButtonsList::Construct(const FArguments& InArgs)
{
	SetRadioButtonStyle(InArgs._Style);	

	OnRadioChoiceChanged = InArgs._OnRadioChoiceChanged;
	
	CurrentChoice = InArgs._CurrentChoice;
	
	CountCheckBox = InArgs._CountCheckBox;

	ChildSlot
	[
		SAssignNew(VerticalBoxMake, SVerticalBox)
	];
	
}

void SRadioButtonsList::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{

	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	BuildButtons();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

ECheckBoxState SRadioButtonsList::IsRadioButtonChecked(int32 RadioButtonID) const
{
	return (CurrentChoice.Get() == RadioButtonID) ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

void SRadioButtonsList::HandleRadioButtonStateChanged(ECheckBoxState NewRadioState, int32 RadioButtonID)
{
	if (NewRadioState == ECheckBoxState::Checked)
	{
		CurrentChoice = RadioButtonID;
		OnRadioChoiceChanged.ExecuteIfBound(RadioButtonID);
	}
}

void SRadioButtonsList::BuildButtons()
{
	if (CurrentCheckBoxCount != CountCheckBox.Get())
	{
		VerticalBoxMake->ClearChildren();

		for (int i = 1; i <= CountCheckBox.Get(); ++i)
		{
			FString Num = FString::FromInt(i);
			VerticalBoxMake->AddSlot()
			[
				CreateRadioButton("Option" + Num, i)
			];
		}
	
		CurrentCheckBoxCount = CountCheckBox.Get();
	}
}

TSharedRef<SWidget> SRadioButtonsList::CreateRadioButton(const FString& RadioText, int32 RadioButtonChoice)
{
	return SNew(SCheckBox)
	.IsChecked(MakeAttributeRaw(this, &SRadioButtonsList::IsRadioButtonChecked, RadioButtonChoice))
	.OnCheckStateChanged(this, &SRadioButtonsList::HandleRadioButtonStateChanged, RadioButtonChoice)
	.Style(CheckBoxStyle)
	[
		SNew(STextBlock)
		.Text(FText::FromString(RadioText))
		.TextStyle(TextBlockStyle)
	];
}

void SRadioButtonsList::SetRadioButtonStyle(const FRadioButtonsStyle* InStyle)
{
	CheckBoxStyle = &InStyle->CheckBoxStyle;
	TextBlockStyle = &InStyle->TextBlockStyle;
}
