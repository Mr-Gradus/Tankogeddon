// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Toolkits/BaseToolkit.h"

class FQuestNpcManageEdModeToolkit : public FModeToolkit
{
public:

	FQuestNpcManageEdModeToolkit();
	
	/** FModeToolkit interface */
	virtual void Init(const TSharedPtr<IToolkitHost>& InitToolkitHost) override;

	/** IToolkit interface */
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual class FEdMode* GetEditorMode() const override;
	virtual TSharedPtr<class SWidget> GetInlineContent() const override { return ToolkitWidget; }

	/** Extra functionality */
	void UpdateColor(FLinearColor NewColor);
	FLinearColor GetColor() const;
	FReply SelectNewColor(const FGeometry&, const FPointerEvent&);
	FReply OnQuestSelection();
	FReply OnButtonClick(FVector InOffset);
	TSharedRef<SWidget> MakeButton(FText InLabel, const FVector InOffset);
	TSharedRef<SWidget> MakeQuestControlButton(FText InLabel);
	

private:
	TSharedPtr<SWidget> ToolkitWidget;


};
