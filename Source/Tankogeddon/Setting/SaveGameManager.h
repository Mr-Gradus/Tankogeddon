#pragma once

#include "CoreMinimal.h"
#include "MySaveGame.h"
#include "UObject/NoExportTypes.h"
#include "SaveGameManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSaveFinish, const FString&, SlotName);

UCLASS()
class TANKOGEDDON_API USaveGameManager : public UObject
{
	GENERATED_BODY()

public:
	
	virtual UWorld* GetWorld() const override
	{
		return HasAnyFlags(RF_ClassDefaultObject) ? nullptr : GetOuter()->GetWorld();
	}

	UPROPERTY(BlueprintAssignable)
	FOnSaveFinish OnGameSaved;

	UPROPERTY(BlueprintAssignable)
	FOnSaveFinish OnGameLoaded;

	void Init();

	UFUNCTION(BlueprintPure)
	bool DoesSaveGameExist(const FString& SlotName) const;

	UFUNCTION(BlueprintCallable)
	void LoadGame(const FString& SlotName);

	UFUNCTION(BlueprintCallable)
	void SaveGame(const FString& SlotName);

	void OnGameLoadedFunc(const FString& SlotName, const int32 UserIndex, USaveGame* SaveGame);


protected:

	void OnGameSavedFunc(const FString& SlotName, const int32 UserIndex, bool bSuccess) const;

	UPROPERTY(BlueprintReadWrite)
	UMySaveGame* CurrentSave;
};
