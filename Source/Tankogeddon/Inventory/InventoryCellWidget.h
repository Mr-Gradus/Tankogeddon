#pragma once

#include "CoreMinimal.h"
#include "InventoryItem.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "InventoryCellWidget.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class MYINVENTORY_API UInventoryCellWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	bool AddItem(const FInventorySlotInfo& Item, const FInventoryItemInfo& ItemInfo);

	void Clear();

	const FInventorySlotInfo& GetItem();

	bool HasItem();

	int32 IndexInInventory = -1;
protected:
	bool bHasItem;

	UPROPERTY(meta = (BindWidgetOptional))
	UImage* ItemImage;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* CountText;

	UPROPERTY()
	FInventorySlotInfo StoredItem;
};
