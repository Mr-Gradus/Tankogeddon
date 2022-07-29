#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LocationMarker.generated.h"

UINTERFACE(MinimalAPI)
class ULocationMarker : public UInterface
{
	GENERATED_BODY()
};

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnLocationReached, AActor* /*LocationMarker*/, AActor* /*OverlappedActor*/);

class QUESTSYSTEM_API ILocationMarker
{
	GENERATED_BODY()

public:

	virtual void NotifyOverlapWithActor(AActor* LocationMarker, AActor* OverlappedActor);

	FOnLocationReached OnLocationReached;
};