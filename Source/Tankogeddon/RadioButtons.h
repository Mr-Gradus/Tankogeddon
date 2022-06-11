#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "UI/SRadioButtonsList.h"
#include "RadioButtons.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRadioChoiceChangedEvent, int32, NewRadioChoice);

UCLASS()
class TANKOGEDDON_API URadioButtons final : public UWidget
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
	void HandleOnRadioChoiceChanged(int32 RadioButtonIndex) const;

	int32 GetCountCheckBox() const;

	int32 GetCurrentChoice() const;

protected:
	TSharedPtr<SRadioButtonsList> MyRadioButtons;

	UPROPERTY(EditAnywhere)
	int32 CountCheckBox;

	UPROPERTY(EditAnywhere)
	int32 CurrentChoice;
};
