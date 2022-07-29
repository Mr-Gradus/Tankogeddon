#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CheckBox.h"
#include "Components/TextBlock.h"
#include "ObjectiveWidget.generated.h"

UCLASS()
class QUESTSYSTEM_API UObjectiveWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	UCheckBox* CompletedCheck;
	
	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* DescriptionText;
	
	void Init(class UObjective* Objective);

	void UpdateCheckBox(UObjective* Objective);   
};
