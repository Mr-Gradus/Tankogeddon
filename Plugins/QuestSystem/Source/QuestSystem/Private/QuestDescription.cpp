#include "QuestDescription.h"
#include "Objective.h"
#include "Quest.h"

void UQuestDescription::Init(AQuest* Quest)
{
	if (Quest)
    {
		if (NameText)
		{
			NameText->SetText(Quest->GetQuestName());
		}
		
		if (DescriptionText)
		{
			DescriptionText->SetText(Quest->Description);
		}
		
		if (ObjectiveWidgetClass && ObjectivesList)
		{
			ObjectivesList->ClearChildren();
			
			for (UObjective* Objective : Quest->GetObjectives())
			{
			    UObjectiveWidget* ObjectiveWidget =  CreateWidget<UObjectiveWidget>(this, ObjectiveWidgetClass);
				
			    ObjectiveWidget->Init(Objective);
				
			    ObjectivesList->AddChild(ObjectiveWidget);
			}
		}
    }
}