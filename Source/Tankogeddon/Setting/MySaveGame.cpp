#include "Tankogeddon/Setting/MySaveGame.h"
#include "NPC_Pawn.h"
#include "Kismet/GameplayStatics.h"

class ANPC_Pawn;

void UMySaveGame::CollectData(UWorld* World)
{
	if (APawn* SavePawn = World->GetFirstPlayerController()->GetPawn())
	{
		
	}
}

void UMySaveGame::ApplyData(UWorld* World)
{
	if (APawn* SavePawn = World->GetFirstPlayerController()->GetPawn())
	{
		
	}
}
