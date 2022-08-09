#include "Tankogeddon/Setting/SaveGameManager.h"
#include "MySaveGame.h"
#include "Kismet/GameplayStatics.h"



void USaveGameManager::Init()
{
	CurrentSave = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	
	ExistingSavedSlots.Empty();
	
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	FString FilePath = FPaths::Combine(FPaths::ProjectContentDir(),	ExistingSavedSlotsFilePath);

	if (PlatformFile.FileExists(*FilePath))
		{
			FString ExistingSavingsArray;
			if (FFileHelper::LoadFileToString(ExistingSavingsArray, *FilePath))
			{
				ExistingSavingsArray.ParseIntoArray(ExistingSavedSlots, TEXT(","));
			}
}


}

bool USaveGameManager::DoesSaveGameExist(const FString& SlotName) 
{
	return UGameplayStatics::DoesSaveGameExist(SlotName, 0);
}

void USaveGameManager::LoadGame(const FString& SlotName)
{
	//UGameplayStatics::LoadGameFromSlot(SlotName, 0);
	UGameplayStatics::AsyncLoadGameFromSlot(SlotName, 0, FAsyncLoadGameFromSlotDelegate::CreateUObject(this,
	&USaveGameManager::OnGameLoadedFromSlotHandle));
}

void USaveGameManager::SaveCurrentGame(const FString& SlotName)
{
	SavePlayer();
	SaveEnemy();

	UGameplayStatics::AsyncSaveGameToSlot(CurrentSave, SlotName, 0, FAsyncSaveGameToSlotDelegate::CreateUObject(this,
	&USaveGameManager::OnGameSavedToSlotHandle));
}



void USaveGameManager::OnGameLoadedFromSlotHandle(const FString& SlotName, const int32 UserIndex, USaveGame* SaveGame)
{
	CurrentSave = Cast<UMySaveGame>(SaveGame);

	LoadPlayer();
	
	if (OnGameLoadedFromSlot.IsBound())
	{
		OnGameLoadedFromSlot.Broadcast(SlotName);
	}

	if (!ExistingSavedSlots.Contains(SlotName))
    {
		ExistingSavedSlots.Add(SlotName);
		CacheExistingSavedSlotsInfo();
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
		CurrentSave->SavedPlayerData.Health = Player->HealthComponent->CurrentHealth;
		CurrentSave->SavedPlayerData.Ammo = Player->GetCannon()->GetAmmo();
	}
	
}

void USaveGameManager::SaveEnemy()
{

	CurrentSave->SavedEnemyTankData.Empty();
	
	for (AActor* Info : GetAllEnemyOfClass(ATankPawn::StaticClass()))
	{
		ATankPawn* SaveTankPawn = Cast<ATankPawn>(Info);
		
		if (IsValid(SaveTankPawn))
		{
			FEnemyTankInfo TempTankData;
			TempTankData.Location = SaveTankPawn->GetActorLocation();
			TempTankData.Rotation = SaveTankPawn->GetActorRotation();
			TempTankData.Health = SaveTankPawn->GetHealthComponent()->GetHealth();
			TempTankData.CannonClass = SaveTankPawn->CannonClass.Get();
			TempTankData.TargetRangeRadius = SaveTankPawn->TargetingRange;
			TempTankData.WaypointTag = SaveTankPawn->WaypointTag;
			
			CurrentSave->SavedEnemyTankData.Add(TempTankData);			
		}
	}

	CurrentSave->SavedEnemyTurretData.Empty();
	
	for (AActor* Info : GetAllEnemyOfClass(ATurret::StaticClass()))
	{
		ATurret* SaveTurret = Cast<ATurret>(Info);
		
		if (IsValid(SaveTurret))
		{
			FEnemyTurretInfo TempTurretData;
			TempTurretData.Location = SaveTurret->GetActorLocation();
			TempTurretData.Health = SaveTurret->GetHealthComponent()->GetHealth();
			TempTurretData.CannonClass = SaveTurret->CannonClass.Get();
			TempTurretData.TargetRangeRadius = SaveTurret->TargetingRange;
			
			CurrentSave->SavedEnemyTurretData.Add(TempTurretData);			
		}
	}
	
}

void USaveGameManager::LoadPlayer()
{
	AParentTankTurret* Player = Cast<AParentTankTurret>(GetWorld()->GetFirstPlayerController()->GetPawn());

	
	if (IsValid(Player))
	{
		Player->SetActorRelativeLocation(CurrentSave->SavedPlayerData.Position, true);
		Player->SetActorRelativeRotation(CurrentSave->SavedPlayerData.Rotation,true);
		Player->HealthComponent->CurrentHealth = CurrentSave->SavedPlayerData.Health;
		Player->Cannon->SetAmmo(CurrentSave->SavedPlayerData.Ammo);	
	}
}

void USaveGameManager::LoadEnemy(TSubclassOf<ATurret> SpawnTurret, TSubclassOf<ATankPawn> SpawnTank) /// Дописать Для турелей и танков отдельные спауны по классам ATurret и ATankPawn 
{
	///Spawn Tanks
	for (AActor* AllEnemyTank : GetAllEnemyOfClass(ATankPawn::StaticClass()))
	{
		ATankPawn* CurrentEnemyTank = Cast<ATankPawn>(AllEnemyTank);

		if (CurrentEnemyTank)
		{
			CurrentEnemyTank->Destroy();
		}
			
		if (CurrentEnemyTank->Cannon)
		{
			CurrentEnemyTank->Cannon->Destroy();
		}
	}

	for (int i = 0; i < CurrentSave->SavedEnemyTankData.Num(); ++i)
	{
		FTransform Transform;
		Transform.SetLocation(CurrentSave->SavedEnemyTankData[i].Location);
		
		ATankPawn* NewTank = GetWorld()->SpawnActorDeferred<ATankPawn>(SpawnTank, Transform,	nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);		
		NewTank->SetActorRotation(CurrentSave->SavedEnemyTankData[i].Rotation);
		NewTank->WaypointTag = CurrentSave->SavedEnemyTankData[i].WaypointTag;
		NewTank->SetupCannon(CurrentSave->SavedEnemyTankData[i].CannonClass);
		//NewTank->TargetingRange(CurrentSave->SavedEnemyTankData[i].TargetRangeRadius);		
		
		UGameplayStatics::FinishSpawningActor(NewTank, Transform);
		
		NewTank->HealthComponent->CurrentHealth = (CurrentSave->SavedEnemyTankData[i].Health);		
	}

	//Spawn turrets
	for (AActor* AllEnemyTurret : GetAllEnemyOfClass(ATurret::StaticClass()))
	{
		ATurret* CurrentEnemyTurret = Cast<ATurret>(AllEnemyTurret);

		if (CurrentEnemyTurret)
		{
			CurrentEnemyTurret->Destroy();
		}
			
		if (CurrentEnemyTurret->Cannon)
		{
			CurrentEnemyTurret->Cannon->Destroy();
		}
	}

	for (int i = 0; i < CurrentSave->SavedEnemyTurretData.Num(); ++i)
	{
		FTransform Transform;
		Transform.SetLocation(CurrentSave->SavedEnemyTurretData[i].Location);
		
		ATurret* NewTurret = GetWorld()->SpawnActorDeferred<ATurret>(SpawnTurret, Transform,	nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);		
		NewTurret->SetupCannon(CurrentSave->SavedEnemyTurretData[i].CannonClass);
		//NewTurret->TargetingRange(CurrentSave->SavedEnemyTurretData[i].TargetRangeRadius);		
		
		UGameplayStatics::FinishSpawningActor(NewTurret, Transform);
		
		NewTurret->HealthComponent->CurrentHealth = (CurrentSave->SavedEnemyTurretData[i].Health);		
	}	
}

void USaveGameManager::CacheExistingSavedSlotsInfo()
{
	FString FilePath = FPaths::Combine(FPaths::ProjectContentDir(), ExistingSavedSlotsFilePath);

	FString ExistingSavingsArray = "";

	for (const FString& Slot : ExistingSavedSlots)
	{
		ExistingSavingsArray += Slot + ",";
	}

	FFileHelper::SaveStringToFile(ExistingSavingsArray, *FilePath, FFileHelper::EEncodingOptions::ForceUnicode,
		&IFileManager::Get(), FILEWRITE_EvenIfReadOnly);
}

TArray<AActor*> USaveGameManager::GetAllEnemyOfClass(TSubclassOf<AActor> EnemyClass)
{
	const FName Tag = "Enemy";
	TArray<AActor*> CountEnemy;
	
	UGameplayStatics::GetAllActorsOfClassWithTag(this, EnemyClass, Tag, CountEnemy);
	
	return CountEnemy;
}

const TArray<FString>& USaveGameManager::GetExistingSavedSlots() const
{
	return ExistingSavedSlots;
}
