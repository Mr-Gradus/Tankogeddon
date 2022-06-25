#pragma once
#include "Engine/DataTable.h"
#include "InventoryItem.generated.h"

UENUM()
enum class EItemType : uint8
{
	IT_Miscellaneous,
	IT_Currency,
	IT_Equipment,
	IT_Consumable
};

UENUM()
enum class EItemRarity : uint8
{
	IR_Common,
	IR_Uncommon,
    IR_Rare,
    IR_Epic,
    IR_Legendary
};

USTRUCT(BlueprintType)
struct FInventorySlotInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FName ItemID;
	UPROPERTY(EditAnywhere)
	int32 Amount;
};


USTRUCT(BlueprintType)
struct FInventoryItemInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	// tech info
	UPROPERTY(EditAnywhere, Category = "General")
	FName ID;
	// text info
	UPROPERTY(EditAnywhere, Category = "General")
	FText Name;

	UPROPERTY(EditAnywhere, Category = "General")
	FText Description;
	// enums
	UPROPERTY(EditAnywhere, Category = "Typing")
	EItemType Type;

	UPROPERTY(EditAnywhere, Category = "Typing")
	EItemRarity Rarity;
	// visual representation
	UPROPERTY(EditAnywhere, Category = "Visual")
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere, Category = "Visual")
	TSoftObjectPtr<UStaticMesh> Mesh;
	// stats
	UPROPERTY(EditAnywhere, Category = "Stats")
	int32 Damage;
	
	UPROPERTY(EditAnywhere, Category = "Stats")
	int32 Armor;
	
	UPROPERTY(EditAnywhere, Category = "Stats")
	int32 Intelligence;
};