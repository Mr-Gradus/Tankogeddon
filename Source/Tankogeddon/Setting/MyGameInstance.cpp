#include "Tankogeddon/Setting/MyGameInstance.h"

void UMyGameInstance::Init()
{
	Super::Init();

	SaveGameManager = NewObject<USaveGameManager>(this);
	SaveGameManager->Init();
}



