#include "InteractionComponent.h"
#include "InteractableObject.h"
#include "Blueprint/UserWidget.h"

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this, &UInteractionComponent::OnComponentBeginOverlapFunc);
	OnComponentEndOverlap.AddDynamic(this, &UInteractionComponent::OnComponentEndOverlapFunc);
}

void UInteractionComponent::OnComponentBeginOverlapFunc(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->Implements<UInteractableObject>())
	{
		return;
	}

	ActorToInteract = OtherActor;

	if (InteractMessage == nullptr && InteractMessageWidget)
	{
		InteractMessage = CreateWidget(GetWorld(), InteractMessageWidget);
	}

	if (InteractMessage && !InteractMessage->IsInViewport() && OtherActor != OverlappedComponent->GetOwner())
	{
		InteractMessage->AddToViewport();
	}
}

void UInteractionComponent::OnComponentEndOverlapFunc(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ActorToInteract == OtherActor)
	{
		ActorToInteract = nullptr;

		if (InteractMessage)
		{
			InteractMessage->RemoveFromViewport();
		}
	}	
}

void UInteractionComponent::Interact()
{
	if (ActorToInteract)
	{
		IInteractableObject::Execute_Interact(ActorToInteract, GetOwner());
	}
}