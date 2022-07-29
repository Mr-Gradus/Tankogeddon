#include "QuestTableRow.h"

#include "ISinglePropertyView.h"
#include "SlateOptMacros.h"
#include "QuestEditor.h"
#include "QuestEditorCommands.h"
#include "Widgets/Layout/SWrapBox.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

TSharedRef<SWidget> SQuestTableRow::GenerateWidgetForColumn(const FName& InColumnName)
{
	FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

	FSinglePropertyParams Params;
	Params.NamePlacement = EPropertyNamePlacement::Hidden;
	
	if (InColumnName == "Name")
	{
		return SNew(SBox)
		.HAlign(HAlign_Fill)
			[
				PropertyModule.CreateSingleProperty(Quest, GET_MEMBER_NAME_CHECKED(AQuest, Name), Params).ToSharedRef()
			];
	}

	if (InColumnName == "Description")
	{
		return SNew(SBox)
		.HAlign(HAlign_Fill)
			[
				PropertyModule.CreateSingleProperty(Quest, GET_MEMBER_NAME_CHECKED(AQuest, Description), Params).ToSharedRef()
			];
	}

	if (InColumnName == "IsStoryQuest")
	{
		return SNew(SBox)
		.HAlign(HAlign_Center)
			[
				PropertyModule.CreateSingleProperty(Quest, GET_MEMBER_NAME_CHECKED(AQuest, bIsStoryQuest), Params).ToSharedRef()
			];
	}

	return SNew(SBox)
		[
			SNew(STextBlock)
			.Text(FText::FromString("unexpected field"))
		];
}

void SQuestTableRow::Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView)
{
	Quest = InArgs._Quest;
	FSuperRowType::Construct(FSuperRowType::FArguments(), InOwnerTableView);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION