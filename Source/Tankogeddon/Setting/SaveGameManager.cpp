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

	UGameplayStatics::AsyncLoadGameFromSlot(SlotName, 0, FAsyncLoadGameFromSlotDelegate::CreateUObject(this,
	&USaveGameManager::OnGameLoadedFromSlotHandle));

	//CurrentSave = Cast<UMySaveGame>(UGameplayStatics::AsyncLoadGameFromSlot(SlotName, 0, OnGameLoadedFromSlot));

	//if (!DoesSaveGameExist(SlotName))
	//{
	//	return;
	//}

	//FAsyncLoadGameFromSlotDelegate LoadDelegate;
	//LoadDelegate.BindUObject(this, &ThisClass::OnGameLoadedFunc);

	//UGameplayStatics::AsyncLoadGameFromSlot(SlotName, 0, LoadDelegate);
}

void USaveGameManager::SaveCurrentGame(const FString& SlotName)
{

	UGameplayStatics::AsyncSaveGameToSlot(CurrentSave, SlotName, 0, FAsyncSaveGameToSlotDelegate::CreateUObject(this,
	&USaveGameManager::OnGameSavedToSlotHandle));
	
	//UGameplayStatics::SaveGameToSlot(CurrentSave, SlotName, 0);
	//FAsyncSaveGameToSlotDelegate SaveDelegate;
	//SaveDelegate.BindUObject(this, &ThisClass::OnGameSavedFunc);

	//CurrentSave->CollectData(GetWorld());

	//UGameplayStatics::AsyncSaveGameToSlot(CurrentSave, SlotName, 0, SaveDelegate);
}

void USaveGameManager::OnGameLoadedFunc(const FString& SlotName, const int32 UserIndex, USaveGame* SaveGame)
{
	CurrentSave = Cast<UMySaveGame>(SaveGame);
	
	OnGameLoadedFromSlot.Broadcast(SlotName);

	CurrentSave->ApplyData(GetWorld());
}

void USaveGameManager::OnGameSavedFunc(const FString& SlotName, const int32 UserIndex, bool bSuccess) const
{
	OnGameSavedToSlot.Broadcast(SlotName);
}

void USaveGameManager::OnGameLoadedFromSlotHandle(const FString& SlotName, const int32 UserIndex, USaveGame* SaveGame)
{
	CurrentSave = Cast<UMySaveGame>(SaveGame);
	
	if (OnGameLoadedFromSlot.IsBound())
	{
		OnGameLoadedFromSlot.Broadcast(SlotName);
	}
}

void USaveGameManager::OnGameSavedToSlotHandle(const FString& SlotName, const int32 UserIndex, bool bSuccess) const
{
	if (OnGameSavedToSlot.IsBound())
	{
		OnGameSavedToSlot.Broadcast(SlotName);
	}
}
