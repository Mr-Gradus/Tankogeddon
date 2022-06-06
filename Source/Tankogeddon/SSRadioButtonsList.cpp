// Fill out your copyright notice in the Description page of Project Settings.


#include "SSRadioButtonsList.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSRadioButtonsList::Construct(const FArguments& InArgs)
{
	
	ChildSlot
	[
		SNew(SVerticalBox)

		+SVerticalBox::Slot()
		[
			SNew(SCheckBox)
			[
				SNew(STextBlock)
				.Text(FText::FromString("Option1"))
			]
		]

		+SVerticalBox::Slot()
		[
			SNew(SCheckBox)
			[
				SNew(STextBlock)
				.Text(FText::FromString("Option2"))
			]
		]

		+SVerticalBox::Slot()
		[
			SNew(SCheckBox)
			[
				SNew(STextBlock)
				.Text(FText::FromString("Option3"))
			]
		]
	];
	
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
