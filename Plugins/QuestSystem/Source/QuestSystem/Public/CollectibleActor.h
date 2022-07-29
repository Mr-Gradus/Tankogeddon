#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CollectibleActor.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCollected, AActor* /*CollectibleActor*/, AActor* /*OverlappedActor*/);

UINTERFACE(MinimalAPI)
class UCollectibleActor : public UInterface
{
	GENERATED_BODY()
};

class QUESTSYSTEM_API ICollectibleActor
{
	GENERATED_BODY()

public:

	virtual void CollectedObjective(AActor* CollectibleActor, AActor* OverlappedActor);

	FOnCollected FOnCollected;
};
