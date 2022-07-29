#include "QuestEditor.h"
#include "QuestEditorStyle.h"
#include "QuestEditorCommands.h"
#include "LevelEditor.h"
#include "Editor/EditorEngine.h"
#include "Engine/Selection.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "QuestTableRow.h"
#include "Quest.h"
#include "NPC_Pawn.h"

static const FName QuestEditorTabName("QuestEditor");

#define LOCTEXT_NAMESPACE "FQuestEditorModule"

void FQuestEditorModule::StartupModule()
{
	FQuestEditorStyle::Initialize();
	FQuestEditorStyle::ReloadTextures();

	FQuestEditorCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FQuestEditorCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FQuestEditorModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FQuestEditorModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(QuestEditorTabName, FOnSpawnTab::CreateRaw(this, &FQuestEditorModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FQuestEditorTabTitle", "QuestEditor"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);

	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");

	TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());	
	MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands,
		FMenuExtensionDelegate::CreateRaw(this, &FQuestEditorModule::AddMenuExtension));

	LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);

	TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender());  
	ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands,
		FToolBarExtensionDelegate::CreateRaw(this ,&FQuestEditorModule::AddToolBarExtension));

	LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);	
}

void FQuestEditorModule::ShutdownModule()
{
	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FQuestEditorStyle::Shutdown();

	FQuestEditorCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(QuestEditorTabName);
}

void FQuestEditorModule::AddMenuExtension(FMenuBuilder& Build)
{
	Build.AddMenuEntry(FQuestEditorCommands::Get().OpenPluginWindow);
}

void FQuestEditorModule::AddToolBarExtension(FToolBarBuilder& Build)
{
	Build.AddToolBarButton(FQuestEditorCommands::Get().OpenPluginWindow);
}

TSharedRef<ITableRow> FQuestEditorModule::GenerateQuestRecord(AQuest* Quest, const TSharedRef<STableViewBase>& Table)
{
	return SNew(SQuestTableRow, Table)
		.Quest(Quest);
}

TSharedRef<SWidget> FQuestEditorModule::GetTableRowHeader(FString RowText)
{
	return SNew(SBorder)
		.HAlign(HAlign_Center)
		.Padding(5.f)
		.Content()
		[
			SNew(STextBlock)
			.Text(FText::FromString(RowText))
		];
}

FReply FQuestEditorModule::OnMoveObjects()
{
	if (GEditor)		
	{
		for (auto Iter = GEditor->GetSelectedActorIterator(); Iter; ++Iter)
		{
			if (AActor* Actor = Cast<AActor>(*Iter))
			{
				Actor->AddActorLocalOffset(FVector(30.f, 20.f, 10.f));
			}
		}
	}

	return FReply::Handled();
}

TSharedRef<SDockTab> FQuestEditorModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	const FText WidgetText = LOCTEXT("MoveObjects", "Move Objects");

	UpdateQuestInfo();

	return SNew(SDockTab)
			.TabRole(NomadTab)
			.OnTabClosed_Raw(this, &FQuestEditorModule::TabClosed)  
			[
				SNew(SBox)
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Top)
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SButton)
						.OnClicked_Raw(this, &FQuestEditorModule::OnMoveObjects)
						[
							SNew(STextBlock)
							.Text(WidgetText)
						]
					]

					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SButton)
						.OnPressed_Raw(this, &FQuestEditorModule::UpdateQuestListView) 
						[
							SNew(STextBlock)
							.Text(FText::FromString("Get Selected Quest Data"))
						]
					]

					+ SVerticalBox::Slot()
					.FillHeight(1.f)
					[
						SNew(SBox)
						[
							SAssignNew(QuestsList, SListView<AQuest*>)
							.ItemHeight(30)
							.ListItemsSource(&Quests)
							.OnGenerateRow_Raw(this, &FQuestEditorModule::GenerateQuestRecord)  //
							.HeaderRow
							(
								SNew(SHeaderRow)
								+ SHeaderRow::Column("Name")[GetTableRowHeader("Name")]
								+ SHeaderRow::Column("Description")[GetTableRowHeader("Description")]
								+ SHeaderRow::Column("IsStoryQuest")[GetTableRowHeader("Is Story Quest")]
							)
						]
					]
				]
			];
}

void FQuestEditorModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(QuestEditorTabName);
}

void FQuestEditorModule::RegisterMenus()
{	
	FToolMenuOwnerScoped OwnerScoped(this);
}

void FQuestEditorModule::UpdateQuestInfo()
{
	Quests.Empty();
	for (FSelectionIterator Iter(*GEditor->GetSelectedActors()); Iter; ++Iter)
	{
		if (auto* Quest = Cast<AQuest>(*Iter))
		{
			Quests.AddUnique(Quest);	
		}

		if (auto* QuestActor = Cast<ANPC_Pawn>(*Iter))
		{
			for (auto* ThisQuest : QuestActor->GetQuests())
			{
				Quests.AddUnique(ThisQuest);
			}
		}
	}

	if (Quests.Num() > 0)
	{
		GEditor->SelectNone(true, true);
		for (auto* Quest : Quests)
		{
			GEditor->SelectActor(Quest, true, false);
		}
	}
}

void FQuestEditorModule::UpdateQuestListView()
{
	UpdateQuestInfo();
	QuestsList->RequestListRefresh();  
}

void FQuestEditorModule::TabClosed(TSharedRef<SDockTab> ClosingTab)
{
	QuestsList.Reset();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FQuestEditorModule, QuestEditor)