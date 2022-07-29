#include "Tankogeddon/Setting/MyGameInstance.h"

#include "SaveGameManager.h"
#include "Kismet/GameplayStatics.h"

void UMyGameInstance::Init()
{
	Super::Init();

	UE_LOG(LogTemp, Warning, TEXT("UMyGameInstance::Init()"));

	SaveGameManager = NewObject<USaveGameManager>(this);
	SaveGameManager->Init();
}

USaveGameManager* UMyGameInstance::GetSaveManager(const UObject* WorldContextObject)
{
	const UMyGameInstance* GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));

	return GameInstance ? GameInstance->SaveGameManager : nullptr;
}


