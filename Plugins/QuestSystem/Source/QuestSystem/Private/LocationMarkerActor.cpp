#include "LocationMarkerActor.h"

void ALocationMarkerActor::OnOverlappedByActor(AActor* OverlappingActor)
{
	NotifyOverlapWithActor(this, OverlappingActor);
}