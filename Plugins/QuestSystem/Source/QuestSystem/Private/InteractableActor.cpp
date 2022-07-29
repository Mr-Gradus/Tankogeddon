#include "InteractableActor.h"

void AInteractableActor::OnInteractionFinished(AActor* InteractInstigator)
{
	NotifyInteractionFinished(this, InteractInstigator);
}
