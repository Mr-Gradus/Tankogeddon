// Copyright Epic Games, Inc. All Rights Reserved.

#include "QuestEditorCommands.h"

#define LOCTEXT_NAMESPACE "FQuestEditorModule"

void FQuestEditorCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "QuestEditor", "Bring up QuestEditor window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
