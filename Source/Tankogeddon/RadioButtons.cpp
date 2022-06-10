
#include "RadioButtons.h"

void URadioButtons::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyRadioButtons.Reset();
}

TSharedRef<SWidget> URadioButtons::RebuildWidget()
{
	MyRadioButtons = SNew(SRadioButtonsList)
	.OnRadioChoiceChanged(BIND_UOBJECT_DELEGATE(FOnRadioChoiceChanged, HandleOnRadioChoiceChanged));
//	.CountCheckBox(GetCountCheckBox);
	
	return MyRadioButtons.ToSharedRef();
}

void URadioButtons::HandleOnRadioChoiceChanged(ERadioChoice NewRadioChoise)
{
	if (OnRadioChoiceChanged.IsBound())
	{
		OnRadioChoiceChanged.Broadcast(NewRadioChoise);
	}
}

//int32 URadioButtons::GetCountCheckBox() const
//{
//	return CountCheckBox;
//}