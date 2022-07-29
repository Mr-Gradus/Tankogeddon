#pragma once

#include "CoreMinimal.h"
#include "ObjectiveWidget.h"
#include "Quest.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "CurrentObjectives.generated.h"

UCLASS()
class QUESTSYSTEM_API UCurrentObjectives : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION()
    void SetCurrentObjectives(AQuest* Quest);

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UObjectiveWidget> ObjectiveWidgetClass;
	
    UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
    UVerticalBox* ObjectivesList;
};
