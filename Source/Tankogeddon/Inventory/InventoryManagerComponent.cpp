#include "InventoryManagerComponent.h"

#include "InventoryWidget.h"
#include "Engine/Engine.h"

UInventoryManagerComponent::UInventoryManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UInventoryManagerComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UInventoryManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UInventoryManagerComponent::Init(UInventoryComponent* InInventoryComponent)
{
	LocalInventoryComponent = InInventoryComponent;
	if (LocalInventoryComponent && InventoryItemsData)
	{
		ensure(InventoryWidgetClass);

		InventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(),	InventoryWidgetClass);

		InventoryWidget->AddToViewport();

		InventoryWidget->Init(FMath::Max(LocalInventoryComponent->GetItemsNum(), MinInventorySize));

		for (auto& Item : LocalInventoryComponent->GetItems())
		{
			FInventoryItemInfo* ItemData = GetItemData(Item.Value.ItemID);
			if (ItemData)
			{
				ItemData->Icon.LoadSynchronous();
				InventoryWidget->AddItem(Item.Value, *ItemData, Item.Key);
			}
		}
	}
}

FInventoryItemInfo * UInventoryManagerComponent::GetItemData(FName ItemID)
{
	if (IsValid(InventoryItemsData))
	{
		return InventoryItemsData->FindRow<FInventoryItemInfo>(ItemID, "");
	}
	return nullptr;
}
