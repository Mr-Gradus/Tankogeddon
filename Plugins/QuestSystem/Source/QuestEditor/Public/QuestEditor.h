#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;
class AQuest;
class STableViewBase;
class ITableRow;

class FQuestEditorModule : public IModuleInterface
{
public:

	virtual void StartupModule() override;
	
	virtual void ShutdownModule() override;
	
	void PluginButtonClicked();

	void AddMenuExtension(FMenuBuilder& Build);

	void AddToolBarExtension(FToolBarBuilder& Build);

	static TSharedRef<SWidget> GetTableRowHeader(FString RowText);

	FReply OnMoveObjects();

	void RegisterMenus();

	TSharedPtr<class FUICommandList> PluginCommands;

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);

	TArray<AQuest*> Quests;

	TSharedPtr<SListView<AQuest*>> QuestsList;

	TSharedRef<ITableRow> GenerateQuestRecord(AQuest* Quest, const TSharedRef<STableViewBase>& Table);

	void UpdateQuestInfo();

	void UpdateQuestListView();

	void TabClosed(TSharedRef<SDockTab> ClosingTab);
};