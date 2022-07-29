#include "Objective.h"

#include "CollectibleActor.h"
#include "InteractableObject.h"
#include "KillableActor.h"
#include "LocationMarker.h"

//============================================ INTERACTION ============================================

UInteractionObjective::UInteractionObjective()
{
	Type = EObjectiveType::Interact;	
}

void UInteractionObjective::ActivateObjective(AActor* Instigator)
{
	if(IInteractableObject* InteractableTarget = Cast<IInteractableObject>(Target))
	{
		InteractableTarget->OnInteractionFinished.AddLambda([this,Instigator]( AActor* InteractableObject, AActor* ActorInteractedWithObject)
		{
			if(bCanBeCompleted && Instigator == ActorInteractedWithObject)
			{
				bIsCompleted =true;

				if(OnObjectiveCompleted.IsBound())
				{
					OnObjectiveCompleted.Broadcast(this);
				}
			}
		});
	}
}

//============================================ LOCATION ============================================

ULocationObjective::ULocationObjective()
{
	Type = EObjectiveType::Location;
}

void ULocationObjective::ActivateObjective(AActor* Instigator)
{
	if (ILocationMarker* LocMarker = Cast<ILocationMarker>(Marker))
	{
		LocMarker->OnLocationReached.AddLambda([this, Instigator](AActor* LocationMarker, AActor* OverlappedActor)
		{
			if (bCanBeCompleted && Instigator == OverlappedActor)
			{
				bIsCompleted = true;
				
				if (OnObjectiveCompleted.IsBound())
				{
				   OnObjectiveCompleted.Broadcast(this);
				}
			}
		});
	}
}

//============================================ COLLECTIBLE ============================================

UCollectibleObjective::UCollectibleObjective()
{
	Type = EObjectiveType::Collect;
}

void UCollectibleObjective::ActivateObjective(AActor* Instigator)
{
	if (ICollectibleActor* Collected = Cast<ICollectibleActor>(CollectibleObject))
	{
		Collected->FOnCollected.AddLambda([this, Instigator](AActor* CollectibleActor, AActor* OverlappedActor)
		{
			if (bCanBeCompleted && Instigator == OverlappedActor)
			{
				bIsCompleted = true;
				
				if (OnObjectiveCompleted.IsBound())
				{
				   OnObjectiveCompleted.Broadcast(this);
				}
			}
		});
	}
}

//============================================ KILLABLE ============================================

UKillableObjective::UKillableObjective()
{
	Type = EObjectiveType::Kill;
}

void UKillableObjective::ActivateObjective(AActor* Instigator)
{
	if (IKillableActor* Killed = Cast<IKillableActor>(KillableObject))
	{
		Killed->FOnKilled.AddLambda([this, Instigator](AActor* KilledActor, AActor* Killer)
		{
			if (bCanBeCompleted && Instigator == Killer)
			{
				bIsCompleted = true;
				
				if (OnObjectiveCompleted.IsBound())
				{
				   OnObjectiveCompleted.Broadcast(this);
				}
			}
		});
	}
}