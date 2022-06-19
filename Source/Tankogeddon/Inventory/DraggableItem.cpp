#include "DraggableItem.h"

#include "FruitDragDropOperation.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/VerticalBox.h"


void UDraggableItem::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (IsValid(NameText))
	{
		NameText->SetText(FruitName);
	}
}

FReply UDraggableItem::NativeOnMouseButtonDown(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}
	return FReply::Handled();
}

void UDraggableItem::NativeOnDragDetected(const FGeometry & InGeometry, const FPointerEvent & InMouseEvent, UDragDropOperation *& OutOperation)
{
	OutOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(UFruitDragDropOperation::StaticClass());

	if (OutOperation)
	{
		UFruitDragDropOperation* FruitDragDropOperation = Cast<UFruitDragDropOperation>(OutOperation);
		FruitDragDropOperation->FruitName = FruitName;
		FruitDragDropOperation->DefaultDragVisual = this;

		SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.7f));
		SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		FruitDragDropOperation->OnDragCancelled.AddDynamic(this, &UDraggableItem::OnDragCancelledHandle);
	}
	else
	{
		Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	}
}

bool UDraggableItem::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UVerticalBox* VerticalBox = Cast<UVerticalBox>(GetParent());
	
	UFruitDragDropOperation* FruitDragDropOperation = Cast<UFruitDragDropOperation>(InOperation);

	if (FruitDragDropOperation && VerticalBox && this != FruitDragDropOperation->DefaultDragVisual)
	{
		int32 IndexDragged = VerticalBox->GetChildIndex(FruitDragDropOperation->DefaultDragVisual);

		if (IndexDragged != -1)
		{
//=============================================================================================================
			//ПОПРОБОВАТЬ ПОЗЖЕ
			//VerticalBox->ReplaceChild(IndexDragged, NewChild);
//=============================================================================================================
			
			VerticalBox->RemoveChildAt(IndexDragged);

			TArray<UWidget*> VBChildren = VerticalBox->GetAllChildren();

			VerticalBox->ClearChildren();

			for (int32 Counter = 0; Counter < VBChildren.Num(); Counter++)
			{
				VerticalBox->AddChildToVerticalBox(VBChildren[Counter]);
				if (VBChildren[Counter] == this)
				{
					VerticalBox->AddChildToVerticalBox(FruitDragDropOperation->DefaultDragVisual);
				}
			}
		}
	}
	return Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);
}

bool UDraggableItem::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (InOperation && InOperation->DefaultDragVisual)
	{
		UUserWidget* DropWidget = Cast<UUserWidget>(InOperation->DefaultDragVisual);
		if (DropWidget)
		{
			DropWidget->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));

			DropWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

void UDraggableItem::OnDragCancelledHandle(UDragDropOperation* Operation)
{
	SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
	SetVisibility(ESlateVisibility::Visible);
}
