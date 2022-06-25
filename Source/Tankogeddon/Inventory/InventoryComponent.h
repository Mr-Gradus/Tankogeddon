#pragma once

#include "CoreMinimal.h"
#include "InventoryItem.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYINVENTORY_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int32, FInventorySlotInfo> Items;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FInventorySlotInfo * GetItem(int32 SlotIndex);
	
    void SetItem(int32 SlotIndex, const FInventorySlotInfo& Item);

	void ClearItem(int32 SlotIndex);

	const TMap<int32, FInventorySlotInfo>& GetItems();

	int32 GetItemsNum() const;

	
};
