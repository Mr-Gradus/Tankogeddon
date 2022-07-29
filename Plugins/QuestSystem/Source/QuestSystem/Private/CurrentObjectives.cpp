#include "CurrentObjectives.h"

void UCurrentObjectives::SetCurrentObjectives(AQuest* Quest)
{
	if (ObjectivesList && ObjectiveWidgetClass)
	{
		ObjectivesList->ClearChildren();
		
		for (UObjective* Objective : Quest->GetObjectives())
		{
			UObjectiveWidget* ObjectiveWidget =	CreateWidget<UObjectiveWidget>(this, ObjectiveWidgetClass);
			
			ObjectiveWidget->Init(Objective);
			
			ObjectivesList->AddChildToVerticalBox(ObjectiveWidget);
		}
	}
}
