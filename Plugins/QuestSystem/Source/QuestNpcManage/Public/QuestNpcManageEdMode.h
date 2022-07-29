// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "EdMode.h"

class FQuestNpcManageEdMode : public FEdMode
{
public:
	const static FEditorModeID EM_QuestNpcManageEdModeId;
public:
	FQuestNpcManageEdMode();
	virtual ~FQuestNpcManageEdMode();

	// FEdMode interface
	virtual void Enter() override;
	virtual void Exit() override;
	//virtual void Tick(FEditorViewportClient* ViewportClient, float DeltaTime) override;
	
	virtual void Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI) override;
	virtual void DrawHUD(FEditorViewportClient* ViewportClient, FViewport* Viewport, const FSceneView* View, FCanvas* Canvas) override;
	virtual void ActorSelectionChangeNotify() override;

	//virtual void ActorSelectionChangeNotify() override;
	bool UsesToolkits() const override;
	// End of FEdMode interface

	void ChangeDebugColors(FLinearColor NewColor) {
		DebugColor = NewColor;  //might need refreshing the visuals?
	}

	FLinearColor GetDebugColor() { return DebugColor; }

protected:
	TArray<AActor*> SelectedActors;

	void UpdateSelectedActors();	
	
private:
	FLinearColor DebugColor = FColor::Yellow;

};
