#include "Tankogeddon/Setting/SaveGameManager.h"
#include "MySaveGame.h"
#include "Kismet/GameplayStatics.h"


void USaveGameManager::Init()
{
	CurrentSave = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
}

bool USaveGameManager::DoesSaveGameExist(const FString& SlotName) const
{
	return UGameplayStatics::DoesSaveGameExist(SlotName, 0);
}

void USaveGameManager::LoadGame(const FString& SlotName)
{
	if (!DoesSaveGameExist(SlotName))
	{
		return;
	}

	FAsyncLoadGameFromSlotDelegate LoadDelegate;
	LoadDelegate.BindUObject(this, &ThisClass::OnGameLoadedFunc);

	UGameplayStatics::AsyncLoadGameFromSlot(SlotName, 0, LoadDelegate);
}

void USaveGameManager::SaveGame(const FString& SlotName)
{
	FAsyncSaveGameToSlotDelegate SaveDelegate;
	SaveDelegate.BindUObject(this, &ThisClass::OnGameSavedFunc);

	CurrentSave->CollectData(GetWorld());

	UGameplayStatics::AsyncSaveGameToSlot(CurrentSave, SlotName, 0, SaveDelegate);
}

void USaveGameManager::OnGameLoadedFunc(const FString& SlotName, const int32 UserIndex, USaveGame* SaveGame)
{
	CurrentSave = Cast<UMySaveGame>(SaveGame);
	
	OnGameLoaded.Broadcast(SlotName);

	CurrentSave->ApplyData(GetWorld());
}

void USaveGameManager::OnGameSavedFunc(const FString& SlotName, const int32 UserIndex, bool bSuccess) const
{
	OnGameSaved.Broadcast(SlotName);
}
