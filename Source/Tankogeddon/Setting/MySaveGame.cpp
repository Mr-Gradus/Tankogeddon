#include "Tankogeddon/Setting/MySaveGame.h"
#include "NPC_Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

class ANPC_Pawn;

void UMySaveGame::CollectData(UWorld* World)
{
	if (APawn* SavePawn = World->GetFirstPlayerController()->GetPawn())
	{
		FMemoryWriter MemoryWriter(Bytes, true);
		
		FObjectAndNameAsStringProxyArchive Ar(MemoryWriter, true);
		Ar.ArIsSaveGame = true;

		SavePawn->Serialize(Ar);

		TArray<AActor*> NpcCollection;
		UGameplayStatics::GetAllActorsOfClass(World, ANPC_Pawn::StaticClass(), NpcCollection); 
		for (auto* Npc : NpcCollection)
		{
			Npc->Serialize(Ar);
		}
		///
	}
}

void UMySaveGame::ApplyData(UWorld* World)
{
	if (APawn* SavePawn = World->GetFirstPlayerController()->GetPawn())
	{
		FMemoryReader MemoryReader(Bytes, true);

		FObjectAndNameAsStringProxyArchive Ar(MemoryReader, true);
		Ar.ArIsSaveGame = true;
		
		SavePawn->Serialize(Ar);

		TArray<AActor*> NpcCollection;
		UGameplayStatics::GetAllActorsOfClass(World, ANPC_Pawn::StaticClass(), NpcCollection); 
		for (auto* Npc : NpcCollection)
		{
			Npc->Serialize(Ar);
		}
		
	}
}
