#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"
#include "InventoryItem.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "InventoryManagerComponent.generated.h"

class UInventoryWidget;
struct FInventoryItemInfo;
class UInventoryComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TANKOGEDDON_API UInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryManagerComponent();

	void Init(UInventoryComponent* InInventoryComponent);

	FInventoryItemInfo* GetItemData(FName ItemID);

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	UInventoryComponent* LocalInventoryComponent;

	UPROPERTY(EditAnywhere)
	UDataTable* InventoryItemsData;

	UPROPERTY()
    UInventoryWidget* InventoryWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;

    UPROPERTY(EditAnywhere)
    int32 MinInventorySize = 20;


public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
