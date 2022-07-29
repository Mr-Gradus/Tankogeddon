// Copyright Epic Games, Inc. All Rights Reserved.
#include "QuestNpcManageEdMode.h"
#include "QuestNpcManageEdModeToolkit.h"
#include "Toolkits/ToolkitManager.h"
#include "EditorModeManager.h"
#include "Engine/Selection.h"

const FEditorModeID FQuestNpcManageEdMode::EM_QuestNpcManageEdModeId = TEXT("EM_QuestNpcManageEdMode");

FQuestNpcManageEdMode::FQuestNpcManageEdMode()
{

}

FQuestNpcManageEdMode::~FQuestNpcManageEdMode()
{

}

void FQuestNpcManageEdMode::Enter()
{
	FEdMode::Enter();

	if (!Toolkit.IsValid() && UsesToolkits())
	{
		Toolkit = MakeShareable(new FQuestNpcManageEdModeToolkit);
		Toolkit->Init(Owner->GetToolkitHost());
	}

	UpdateSelectedActors();
}

void FQuestNpcManageEdMode::Exit()
{
	if (Toolkit.IsValid())
	{
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}

	// Call base Exit method to ensure proper cleanup
	FEdMode::Exit();
}

void FQuestNpcManageEdMode::Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI)
{
	// Forming a bounding box for each in stored actors
	for (AActor* BoundedActor : SelectedActors)
	{
		DrawWireBox(
			PDI,
			BoundedActor->GetComponentsBoundingBox(true),
			DebugColor,
			1);
	}

	FEdMode::Render(View, Viewport, PDI);
}

void FQuestNpcManageEdMode::DrawHUD(FEditorViewportClient* ViewportClient, FViewport* Viewport, const FSceneView* View, FCanvas* Canvas)
{
	FEdMode::DrawHUD(ViewportClient, Viewport, View, Canvas);

	// Rendering actor misc data
	for (AActor* SelectedActor : SelectedActors)
	{
		if (Canvas)
		{
			FBox Bounds = SelectedActor->GetComponentsBoundingBox(true);
;
			FVector DrawPos = Bounds.GetCenter() + FVector(0.f, 0.f, Bounds.GetExtent().Z);
			FVector2D PixelLocation;

			View->ScreenToPixel(View->WorldToScreen(DrawPos), PixelLocation);
			PixelLocation /= ViewportClient->GetDPIScale();
			UFont* RenderFont = GEngine->GetSmallFont();
			Canvas->DrawShadowedText(PixelLocation.X, PixelLocation.Y,
				FText::FromString(SelectedActor->GetName()),
				RenderFont, DebugColor);
		}
	}
}

void FQuestNpcManageEdMode::ActorSelectionChangeNotify()
{
	FEdMode::ActorSelectionChangeNotify();

	UpdateSelectedActors();
}

bool FQuestNpcManageEdMode::UsesToolkits() const
{
	return true;
}

void FQuestNpcManageEdMode::UpdateSelectedActors()
{
	SelectedActors.Empty();

	// saving a selection of actors to private variable.
	USelection* SelectedActorSet = GEditor->GetSelectedActors();
	for (FSelectionIterator Iter(*SelectedActorSet); Iter; ++Iter)
	{
		AActor* LevelActor = Cast<AActor>(*Iter);
		if (LevelActor && !SelectedActors.Contains(LevelActor))
		{
			SelectedActors.Add(LevelActor);
		}
	}


}



