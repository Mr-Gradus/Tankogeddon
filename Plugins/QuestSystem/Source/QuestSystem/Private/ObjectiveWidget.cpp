#include "ObjectiveWidget.h"
#include "Objective.h"

void UObjectiveWidget::Init(UObjective* Objective)
{
	if (Objective)
    {
	    if (CompletedCheck)
	    {
			CompletedCheck->SetIsChecked(Objective->bIsCompleted);	    	
	    }
		
	    if (DescriptionText)
	    {
	    	DescriptionText->SetText(Objective->Description);
	    }

		Objective->OnObjectiveCompleted.AddUObject(this, &ThisClass::UpdateCheckBox);
    }	
}

void UObjectiveWidget::UpdateCheckBox(UObjective* Objective)
{
	CompletedCheck->SetIsChecked(Objective->bIsCompleted);	
}