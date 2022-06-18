#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateWidgetStyle.h"
#include "Styling/SlateWidgetStyleContainerBase.h"
#include "Styling/SlateTypes.h"

#include "ButtonsWidgetStyle.generated.h"

USTRUCT()
struct TANKOGEDDON_API FButtonsStyle : public FSlateWidgetStyle
{
	GENERATED_USTRUCT_BODY()

	FButtonsStyle();
	virtual ~FButtonsStyle() override;

	virtual void GetResources(TArray<const FSlateBrush*>& OutBrushes) const override;

	static const FName TypeName;

	virtual const FName GetTypeName() const override { return TypeName; };

	static const FButtonsStyle& GetDefault();

	UPROPERTY(EditAnywhere, Category = Appearance)
	FButtonStyle ButtonStyle;  
};

/**
 */
UCLASS(hidecategories=Object, MinimalAPI)
class UButtonsWidgetStyle : public USlateWidgetStyleContainerBase
{
	GENERATED_BODY()

public:
	/** The actual data describing the widget appearance. */
	UPROPERTY(Category=Appearance, EditAnywhere, meta=(ShowOnlyInnerProperties))
	FButtonsStyle WidgetStyle;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return static_cast< const struct FSlateWidgetStyle* >( &WidgetStyle );
	}
};
