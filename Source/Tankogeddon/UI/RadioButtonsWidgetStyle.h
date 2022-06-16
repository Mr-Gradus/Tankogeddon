#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateWidgetStyle.h"
#include "Styling/SlateWidgetStyleContainerBase.h"
#include "Styling/SlateTypes.h"
#include "RadioButtonsWidgetStyle.generated.h"

/**
 * 
 */
USTRUCT()
struct TANKOGEDDON_API FRadioButtonsStyle : public FSlateWidgetStyle
{
	GENERATED_USTRUCT_BODY()

	FRadioButtonsStyle();

	virtual ~FRadioButtonsStyle();

	virtual void GetResources(TArray<const FSlateBrush*>& OutBrushes) const override;

	static const FName TypeName;

	virtual const FName GetTypeName() const override { return TypeName; };

	static const FRadioButtonsStyle& GetDefault();

	UPROPERTY(EditAnywhere, Category= Appearance)
	FCheckBoxStyle CheckBoxStyle;

	UPROPERTY(EditAnywhere, Category= Appearance)
	FTextBlockStyle TextBlockStyle;
};

/**
 */
UCLASS(hidecategories=Object, MinimalAPI)
class URadioButtonsWidgetStyle : public USlateWidgetStyleContainerBase
{
	GENERATED_BODY()

public:
	/** The actual data describing the widget appearance. */
	UPROPERTY(Category=Appearance, EditAnywhere, meta=(ShowOnlyInnerProperties))
	FRadioButtonsStyle WidgetStyle;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return static_cast< const struct FSlateWidgetStyle* >( &WidgetStyle );
	}
};
