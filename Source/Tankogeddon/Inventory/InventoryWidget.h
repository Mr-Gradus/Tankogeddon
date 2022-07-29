#pragma once

#include "CoreMinimal.h"
#include "InventoryCellWidget.h"
#include "InventoryItem.h"
#include "Blueprint/UserWidget.h"
#include "Components/UniformGridPanel.h"
#include "InventoryWidget.generated.h"


UCLASS()
class TANKOGEDDON_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void Init(int32 ItemsNum);
	
	bool AddItem(const FInventorySlotInfo& Item, const FInventoryItemInfo& ItemInfo, int32 SlotPosition = -1);

protected:
	UPROPERTY(meta = (BindWidgetOptional))
	UUniformGridPanel* ItemCellsGrid;

	UPROPERTY(EditDefaultsOnly)
	int32 ItemsInRow = 5;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventoryCellWidget> CellWidgetClass;

	UPROPERTY()
	TArray<UInventoryCellWidget*> CellWidgets;

	UPROPERTY(meta = (BindWidgetOptional))
	UInventoryCellWidget* GoldCell;

	UInventoryCellWidget * CreateCellWidget();
};
