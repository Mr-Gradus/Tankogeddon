#include "MyInventory/Inventory/InventoryCellWidget.h"

bool UInventoryCellWidget::AddItem(const FInventorySlotInfo& Item, const FInventoryItemInfo& ItemInfo)
{
	if (bHasItem)
	{
		return false;
	}
	
	if (ItemImage)
	{
		ItemImage->SetBrushFromTexture(ItemInfo.Icon.Get());
		ItemImage->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f));
	}
	
	if (CountText)
	{
		CountText->SetText(FText::FromString(FString::FromInt(Item.Amount)));
	}
	bHasItem = true;
	StoredItem = Item;
	return true;
}

void UInventoryCellWidget::Clear()
{
	if (ItemImage)
	{
		ItemImage->SetBrush(FSlateBrush());
		ItemImage->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.f));
	}
	if (CountText)
	{
		CountText->SetText(FText::FromString("0"));
	}
	bHasItem = false;
	StoredItem = FInventorySlotInfo();
}

const FInventorySlotInfo& UInventoryCellWidget::GetItem()
{
	return StoredItem;
}

bool UInventoryCellWidget::HasItem()
{
	return bHasItem;
}