#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "UI/SRadioButtonsList.h"
#include "RadioButtons.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRadioChoiceChangedEvent, ERadioChoice, NewRadioChoise);

UCLASS()
class TANKOGEDDON_API URadioButtons : public UWidget
{
	GENERATED_BODY()

public:
	//~ Begin UVisual Interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	//~ End UVisual Interface

	/** Called when radio choice changed by click */
	UPROPERTY(BlueprintAssignable, Category = "RadioButtons|Event")
	FOnRadioChoiceChangedEvent OnRadioChoiceChanged;

protected:
	//~ Begin UWidget Interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	//~ End UWidget Interface

	/** Handle the actual click event from slate and forward it on */
	void HandleOnRadioChoiceChanged(ERadioChoice NewRadioChoice);
	//int32 GetCountCheckBox() const;

protected:
	TSharedPtr<SRadioButtonsList> MyRadioButtons;

	UPROPERTY(BlueprintReadWrite, Category= "RadioButtons")
	int32 CountCheckBox;
};
