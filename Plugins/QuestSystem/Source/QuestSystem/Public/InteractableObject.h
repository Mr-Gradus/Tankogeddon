#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableObject.generated.h"

UINTERFACE(MinimalAPI)
class UInteractableObject : public UInterface
{
	GENERATED_BODY()
};

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnInteractionFinished, AActor* /*InteractObject*/, AActor* /*InteractInstigator*/ )

class QUESTSYSTEM_API IInteractableObject
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Interact(AActor* InteractInstigator);

	UFUNCTION()
	virtual void Interact_Implementation(AActor* InteractInstigator) {};

	virtual void NotifyInteractionFinished(AActor* InteractObject, AActor* InteractInstigator);

	FOnInteractionFinished OnInteractionFinished;
};
