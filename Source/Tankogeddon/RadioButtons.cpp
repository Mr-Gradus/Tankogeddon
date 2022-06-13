
#include "RadioButtons.h"


void URadioButtons::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyRadioButtons.Reset();
}

TSharedRef<SWidget> URadioButtons::RebuildWidget()
{
	MyRadioButtons = SNew(SRadioButtonsList)
	.OnRadioChoiceChanged(BIND_UOBJECT_DELEGATE(FOnRadioChoiceChanged, HandleOnRadioChoiceChanged))
	.CountCheckBox(GetCountCheckBox())
	.CurrentChoice(GetCurrentChoice());
	
		
	return MyRadioButtons.ToSharedRef();
}

void URadioButtons::HandleOnRadioChoiceChanged(int32 RadioButtonChoice) const
{
	if (OnRadioChoiceChanged.IsBound())
	{
		OnRadioChoiceChanged.Broadcast(RadioButtonChoice);
	}
}

int32 URadioButtons::GetCountCheckBox() const
{
	return CountCheckBox;
}

int32 URadioButtons::GetCurrentChoice() const
{
	return CurrentChoice;
}

void URadioButtons::AddCountCheckBox(int32 AddValue)
{
	MyRadioButtons->CountCheckBox = AddValue;
	
}


