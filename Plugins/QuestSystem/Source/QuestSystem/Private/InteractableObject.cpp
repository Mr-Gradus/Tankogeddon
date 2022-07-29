#include "InteractableObject.h"

void IInteractableObject::NotifyInteractionFinished(AActor* InteractObject, AActor* InteractInstigator)
{
	OnInteractionFinished.Broadcast(InteractObject, InteractInstigator);
}
