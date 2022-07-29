#include "CollectibleActor.h"

void ICollectibleActor::CollectedObjective(AActor* CollectibleActor, AActor* OverlappedActor)
{
	if(FOnCollected.IsBound())
	{
		FOnCollected.Broadcast(CollectibleActor, OverlappedActor);
	}
}
