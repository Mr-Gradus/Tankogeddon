#pragma once

#include "CoreMinimal.h"
#include "MySaveGame.h"
#include "Tankogeddon/ParentTankTurret.h"
#include "UObject/NoExportTypes.h"
#include "SaveGameManager.generated.h"

class USaveGame;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameFromSlotAction, const FString&, SlotName);

UCLASS()
class TANKOGEDDON_API USaveGameManager : public UObject
{
	GENERATED_BODY()

public:
	void Init();
	
	UPROPERTY(BlueprintAssignable)
	FOnGameFromSlotAction OnGameLoadedFromSlot;

	UPROPERTY(BlueprintAssignable)
	FOnGameFromSlotAction OnGameSavedToSlot;


	UFUNCTION(BlueprintCallable)
	bool DoesSaveGameExist(const FString& SlotName);

	UFUNCTION(BlueprintCallable)
	void LoadGame(const FString& SlotName);

	UFUNCTION(BlueprintCallable)
	void SaveCurrentGame(const FString& SlotName);

	void OnGameLoadedFunc(const FString& SlotName, const int32 UserIndex, USaveGame* SaveGame);

	void OnGameSavedFunc(const FString& SlotName, const int32 UserIndex, bool bSuccess);

	TArray<AActor*> GetAllEnemyOfClass(TSubclassOf<AActor> EnemyClass);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn tanks params")
	TSubclassOf<AParentTankTurret> SpawnClass;

protected:
	UPROPERTY(BlueprintReadWrite)
	UMySaveGame* CurrentSave;

	void OnGameLoadedFromSlotHandle(const FString& SlotName, const int32 UserIndex, USaveGame* SaveGame);

	void OnGameSavedToSlotHandle(const FString& SlotName, const int32 UserIndex, bool bSuccess) const;
	void SavePlayer();
	void SaveEnemy();
	void LoadPlayer();
	void LoadEnemy();
};
