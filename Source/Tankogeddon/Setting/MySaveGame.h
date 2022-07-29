#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
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

	UPROPERTY()
	TArray<uint8> Bytes;

	void CollectData(UWorld* World);

	void ApplyData(UWorld* World);
};
