#include "Tankogeddon/UI/ButtonsWidgetStyle.h"
#include "Styling/CoreStyle.h"


FButtonsStyle::FButtonsStyle()
{
	ButtonStyle = FCoreStyle::Get().GetWidgetStyle<FButtonStyle>("ButtonStyle");
}

FButtonsStyle::~FButtonsStyle()
{
}

const FName FButtonsStyle::TypeName(TEXT("FButtonsStyle"));

const FButtonsStyle& FButtonsStyle::GetDefault()
{
	static FButtonsStyle Default;
	return Default;
}

void FButtonsStyle::GetResources(TArray<const FSlateBrush*>& OutBrushes) const
{
	// Add any brush resources here so that Slate can correctly atlas and reference them
}

