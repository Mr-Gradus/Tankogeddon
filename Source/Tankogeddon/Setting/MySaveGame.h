#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Tankogeddon/Armory/Cannon.h"
#include "MySaveGame.generated.h"

USTRUCT(BlueprintType)
struct FPlayerInfo
{
	GENERATED_BODY()

	UPROPERTY()
	int32 Ammo;

	UPROPERTY()
	float Health;

	UPROPERTY()
	FVector Position;

	UPROPERTY()
	FRotator Rotation;
};

USTRUCT(BlueprintType)
struct FEnemyTankInfo
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<ACannon> CannonClass;

	UPROPERTY(BlueprintReadWrite)
	FName WaypointTag;

	UPROPERTY(BlueprintReadWrite)
	float TargetRangeRadius;
	
	UPROPERTY(BlueprintReadWrite)
	FVector Location;

	UPROPERTY(BlueprintReadWrite)
	FRotator Rotation;

	UPROPERTY(BlueprintReadWrite)
	float Health;
};

USTRUCT(BlueprintType)
struct FEnemyTurretInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<ACannon> CannonClass;

	UPROPERTY(BlueprintReadWrite)
	float TargetRangeRadius;

	UPROPERTY(BlueprintReadWrite)
	FVector Location;

	UPROPERTY(BlueprintReadWrite)
	float Health;
};

UCLASS()
class TANKOGEDDON_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	float TestFloat;

	UPROPERTY(BlueprintReadWrite)
	FPlayerInfo SavedPlayerData;

	UPROPERTY(BlueprintReadWrite)
	TArray<FEnemyTankInfo> SavedEnemyTankData;

	UPROPERTY(BlueprintReadWrite)
	TArray<FEnemyTurretInfo> SavedEnemyTurretData;

};
