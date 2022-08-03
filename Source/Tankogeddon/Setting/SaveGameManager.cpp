#include "Tankogeddon/Setting/SaveGameManager.h"
#include "MySaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Tankogeddon/ParentTankTurret.h"


void USaveGameManager::Init()
{
	CurrentSave = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
}

bool USaveGameManager::DoesSaveGameExist(const FString& SlotName) 
{
	return UGameplayStatics::DoesSaveGameExist(SlotName, 0);
}

void USaveGameManager::LoadGame(const FString& SlotName)
{

	LoadPlayer();
	
	UGameplayStatics::AsyncLoadGameFromSlot(SlotName, 0, FAsyncLoadGameFromSlotDelegate::CreateUObject(this,
	&USaveGameManager::OnGameLoadedFromSlotHandle));

}

void USaveGameManager::SaveCurrentGame(const FString& SlotName)
{
	
	SavePlayer();

	UGameplayStatics::AsyncSaveGameToSlot(CurrentSave, SlotName, 0, FAsyncSaveGameToSlotDelegate::CreateUObject(this,
	&USaveGameManager::OnGameSavedToSlotHandle));
}

void USaveGameManager::OnGameLoadedFunc(const FString& SlotName, const int32 UserIndex, USaveGame* SaveGame)
{
	CurrentSave = Cast<UMySaveGame>(SaveGame);
	
	OnGameLoadedFromSlot.Broadcast(SlotName);

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

void USaveGameManager::SavePlayer()
{
	AParentTankTurret* Player = Cast<AParentTankTurret>(GetWorld()->GetFirstPlayerController()->GetPawn());
	
	if (IsValid(Player))
	{
		CurrentSave->SavedPlayerData.Position = Player->GetActorLocation();
		CurrentSave->SavedPlayerData.Rotation = Player->GetActorRotation();
		CurrentSave->SavedPlayerData.Health = Player->GetHealthComponent()->GetHealth();
		CurrentSave->SavedPlayerData.Ammo = Player->GetCannon()->GetAmmo();
	}
	
}

void USaveGameManager::LoadPlayer()
{
	AParentTankTurret* Player = Cast<AParentTankTurret>(GetWorld()->GetFirstPlayerController()->GetPawn());
	
	if (IsValid(Player))
	{
		Player->SetActorRelativeLocation(CurrentSave->SavedPlayerData.Position, true);
		Player->SetActorRelativeRotation(CurrentSave->SavedPlayerData.Rotation,true);
		Player->OnHealthChanged(CurrentSave->SavedPlayerData.Health);
		Player->Cannon->SetAmmo(CurrentSave->SavedPlayerData.Ammo);	
	}
}