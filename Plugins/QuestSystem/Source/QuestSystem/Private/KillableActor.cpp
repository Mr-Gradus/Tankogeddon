#include "KillableActor.h"

void IKillableActor::KilledActor(AActor* DestroyedPawn, AActor* Killer)
{
	if(FOnKilled.IsBound())
	{
		FOnKilled.Broadcast(DestroyedPawn, Killer);
	}
}
