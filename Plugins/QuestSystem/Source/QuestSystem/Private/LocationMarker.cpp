#include "LocationMarker.h"

void ILocationMarker::NotifyOverlapWithActor(AActor* LocationMarker, AActor* OverlappedActor)
{
	if(OnLocationReached.IsBound())
	{
		OnLocationReached.Broadcast(LocationMarker, OverlappedActor);
	}
}