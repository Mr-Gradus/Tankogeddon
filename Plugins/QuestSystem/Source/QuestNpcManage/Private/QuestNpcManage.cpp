// Copyright Epic Games, Inc. All Rights Reserved.

#include "QuestNpcManage.h"
#include "QuestNpcManageEdMode.h"

#define LOCTEXT_NAMESPACE "FQuestNpcManageModule"

void FQuestNpcManageModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FEditorModeRegistry::Get().RegisterMode<FQuestNpcManageEdMode>(FQuestNpcManageEdMode::EM_QuestNpcManageEdModeId, LOCTEXT("QuestNpcManageEdModeName", "QuestNpcManageEdMode"), FSlateIcon(), true);
}

void FQuestNpcManageModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FEditorModeRegistry::Get().UnregisterMode(FQuestNpcManageEdMode::EM_QuestNpcManageEdModeId);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FQuestNpcManageModule, QuestNpcManage)