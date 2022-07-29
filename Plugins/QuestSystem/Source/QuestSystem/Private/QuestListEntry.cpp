#include "QuestListEntry.h"

void UQuestListEntry::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (SelectQuestButton)
	{
		SelectQuestButton->OnReleased.AddDynamic(this, &UQuestListEntry::SelectQuest);
	}
}

void UQuestListEntry::SetQuestText(const FText& Text)
{
	if (QuestText)
	{
		QuestText->SetText(Text);
	}
}

void UQuestListEntry::SetIsSelected(bool IsSelected)
{
	if (SelectionBorder)
	{
		SelectionBorder->SetBrushColor(IsSelected ?	FLinearColor(1.f, 1.f, 1.f, 1.f) : FLinearColor(1.f, 1.f, 1.f, 0.f));
	}
}

void UQuestListEntry::SelectQuest()
{
	OnQuestChosen.ExecuteIfBound();	
}

