#pragma once

#include <CoreMinimal.h>
#include "Quest.h"

class SQuestTableRow : public SMultiColumnTableRow<AQuest*>
{
public:
	SLATE_BEGIN_ARGS(SQuestTableRow) {}
	SLATE_ARGUMENT(AQuest*, Quest)
	SLATE_END_ARGS()

	virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& InColumnName) override;

	void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView);

	AQuest* Quest = nullptr;
};

