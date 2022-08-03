#include "Tankogeddon/Setting/MyGameInstance.h"

void UMyGameInstance::Init()
{
	Super::Init();

	UE_LOG(LogTemp, Warning, TEXT("UMyGameInstance::Init()"));

	SaveGameManager = NewObject<USaveGameManager>(this);
	SaveGameManager->Init();
}



