// Copyright Epic Games, Inc. All Rights Reserved.

#include "QuestNpcManageEdModeToolkit.h"
#include "QuestNpcManageEdMode.h"
#include "Engine/Selection.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Colors/SColorBlock.h"
#include "Widgets/Colors/SColorPicker.h"
#include "EditorModeManager.h"
#include "Quest.h"
#include "NPC_Pawn.h"
#include "Kismet/GameplayStatics.h"

#define LOCTEXT_NAMESPACE "FQuestNpcManageEdModeToolkit"

FQuestNpcManageEdModeToolkit::FQuestNpcManageEdModeToolkit()
{
}

void FQuestNpcManageEdModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost)
{
	struct Locals
	{
		static bool IsWidgetEnabled()
		{
			return GEditor->GetSelectedActors()->Num() != 0;
		}
	};

	const float Factor = 256.0f;

	SAssignNew(ToolkitWidget, SBorder)
		.HAlign(HAlign_Center)
		.Padding(25)
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			.HAlign(HAlign_Center)
			.AutoHeight()
			[

				SNew(SBorder)
				.HAlign(HAlign_Center)
				.IsEnabled_Static(&Locals::IsWidgetEnabled)
				[
					SNew(SVerticalBox)

					+ SVerticalBox::Slot()
					.AutoHeight()
					.HAlign(HAlign_Center)
					.Padding(50)
					[
						SNew(STextBlock)
						.AutoWrapText(true)
						.Text(LOCTEXT("HelperLabel", "Select some actors and move them around using buttons below"))
					]
					+ SVerticalBox::Slot()
					.HAlign(HAlign_Center)
					.AutoHeight()
					[
						MakeButton(LOCTEXT("UpButtonLabel", "Up"), FVector(0, 0, Factor))
					]

					+ SVerticalBox::Slot()
					.HAlign(HAlign_Center)
					.AutoHeight()
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							MakeButton(LOCTEXT("LeftButtonLabel", "Left"), FVector(0, -Factor, 0))
						]
						+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							MakeButton(LOCTEXT("RightButtonLabel", "Right"), FVector(0, Factor, 0))
						]
					]
				
					+ SVerticalBox::Slot()
					.HAlign(HAlign_Center)
					.AutoHeight()
					[
						MakeButton(LOCTEXT("DownButtonLabel", "Down"), FVector(0, 0, -Factor))
					]
				]
			]

			+ SVerticalBox::Slot()
			.HAlign(HAlign_Center)
			.AutoHeight()
			[

				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.AutoHeight()
				.HAlign(HAlign_Center)
				.Padding(5)
				[
					SNew(STextBlock)
					.AutoWrapText(true)
					.Text(LOCTEXT("HelperLabel2", "Quest location and recoloring"))
				]
				
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					MakeQuestControlButton(LOCTEXT("ActorLocateButtonLabel", "Get quest objects"))
				]

				+SVerticalBox::Slot()
				.HAlign(HAlign_Center)
				.AutoHeight()
				[
					SNew(SColorBlock)
					.Size(FVector2D(50.f, 50.f))
					.Color_Raw(this, &FQuestNpcManageEdModeToolkit::GetColor)		//binding to specific color
					.OnMouseButtonDown_Raw(this, &FQuestNpcManageEdModeToolkit::SelectNewColor)
				]
			]
		];
			
	FModeToolkit::Init(InitToolkitHost);
}

FReply FQuestNpcManageEdModeToolkit::OnButtonClick(FVector InOffset)
{
	USelection* SelectedActors = GEditor->GetSelectedActors();

	// Let editor know that we're about to do something that we want to undo/redo
	GEditor->BeginTransaction(LOCTEXT("MoveActorsTransactionName", "MoveActors"));

	// For each selected actor
	for (FSelectionIterator Iter(*SelectedActors); Iter; ++Iter)
	{
		if (AActor* LevelActor = Cast<AActor>(*Iter))
		{
			// Register actor in opened transaction (undo/redo)
			LevelActor->Modify();
			// Move actor to given location
			LevelActor->TeleportTo(LevelActor->GetActorLocation() + InOffset, FRotator(0, 0, 0));
		}
	}

	// We're done moving actors so close transaction
	GEditor->EndTransaction();

	return FReply::Handled();
}

TSharedRef<SWidget> FQuestNpcManageEdModeToolkit::MakeButton(FText InLabel, const FVector InOffset)
{
	return SNew(SButton)
		.Text(InLabel)
		.OnClicked_Raw(this, &FQuestNpcManageEdModeToolkit::OnButtonClick, InOffset);
}

TSharedRef<SWidget> FQuestNpcManageEdModeToolkit::MakeQuestControlButton(FText InLabel)
{
	return SNew(SButton)
		.Text(InLabel)
		.OnClicked_Raw(this, &FQuestNpcManageEdModeToolkit::OnQuestSelection);
}

FReply FQuestNpcManageEdModeToolkit::OnQuestSelection()
{
	USelection* SelectedActors = GEditor->GetSelectedActors();

	// Displaying objectives for selected NPCs

	if (SelectedActors->CountSelections(ANPC_Pawn::StaticClass()) > 0)
	{
		GLog->Log("looking into selection");

		for (FSelectionIterator Iter(*SelectedActors); Iter; ++Iter)
		{
			if (auto* LevelActor = Cast<ANPC_Pawn>(*Iter))
			{
				for (AActor* ThisObjective : LevelActor->GetAllObjectiveComps())
				{
					GEditor->SelectActor(ThisObjective, true, false);
					//GEditor->SelectComponent()???
				}
			}
		}
	}
	else			// If seleciton is empty, select NPCs with quests
	{
		TArray<AActor*> ActorCollection;
		UGameplayStatics::GetAllActorsOfClass(GEditor->GetWorld(), ANPC_Pawn::StaticClass(), ActorCollection);

		auto* CurrentWorld = GEditor->GetEditorWorldContext().World();
		for (TActorIterator<ANPC_Pawn> Iter(CurrentWorld, ANPC_Pawn::StaticClass()); Iter; ++Iter)
		//for (AActor* ThisActor : ActorCollection)
		{
			GLog->Log("found an actor " + Iter->GetActorLabel());
			// check for quests in attached actors
			if (Iter->HasQuests())
			{
				GEditor->SelectActor(*Iter, true, false);
			}
		}
	}

	FQuestNpcManageEdMode* CurrentMode = StaticCast<FQuestNpcManageEdMode*>(GetEditorMode());
	CurrentMode->ActorSelectionChangeNotify();

	return FReply::Handled();
}


FLinearColor FQuestNpcManageEdModeToolkit::GetColor() const
{
	if (!GetEditorMode()->GetID().IsNone())
	{
		FQuestNpcManageEdMode* CurrentMode = StaticCast<FQuestNpcManageEdMode*>(GetEditorMode());
		return CurrentMode->GetDebugColor();
	}

	return FColor::Magenta;		// if missing color for some reason
}

FReply FQuestNpcManageEdModeToolkit::SelectNewColor(const FGeometry&, const FPointerEvent&)
{
	FColorPickerArgs PickerArgs;
	PickerArgs.bUseAlpha = false;
	PickerArgs.bIsModal = true;
	PickerArgs.InitialColorOverride = GetColor(); 
	PickerArgs.OnColorCommitted = FOnLinearColorValueChanged::CreateSP(this, &FQuestNpcManageEdModeToolkit::UpdateColor);

	OpenColorPicker(PickerArgs);

	
	return FReply::Handled();
}

void FQuestNpcManageEdModeToolkit::UpdateColor(FLinearColor NewColor)
{
	if (!GetEditorMode()->GetID().IsNone())
	{
		FQuestNpcManageEdMode* CurrentMode = StaticCast<FQuestNpcManageEdMode*>(GetEditorMode());
		CurrentMode->ChangeDebugColors(NewColor);
	}
}

FName FQuestNpcManageEdModeToolkit::GetToolkitFName() const
{
	return FName("QuestNpcManageEdMode");
}

FText FQuestNpcManageEdModeToolkit::GetBaseToolkitName() const
{
	return NSLOCTEXT("QuestNpcManageEdModeToolkit", "DisplayName", "QuestNpcManageEdMode Tool");
}

class FEdMode* FQuestNpcManageEdModeToolkit::GetEditorMode() const
{
	return GLevelEditorModeTools().GetActiveMode(FQuestNpcManageEdMode::EM_QuestNpcManageEdModeId);
}

#undef LOCTEXT_NAMESPACE
