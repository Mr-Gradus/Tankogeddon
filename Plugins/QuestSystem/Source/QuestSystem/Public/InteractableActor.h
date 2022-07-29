#pragma once

#include "CoreMinimal.h"
#include "InteractableObject.h"
#include "GameFramework/Actor.h"
#include "InteractableActor.generated.h"

UCLASS()
class QUESTSYSTEM_API AInteractableActor : public AActor, public IInteractableObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void OnInteractionFinished(AActor* InteractInstigator);
};