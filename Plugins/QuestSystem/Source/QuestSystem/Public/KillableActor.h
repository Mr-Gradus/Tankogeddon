#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "KillableActor.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnKilled, AActor* /*Destroyed Pawn*/, AActor* /*Killer*/)

UINTERFACE(MinimalAPI)
class UKillableActor : public UInterface
{
	GENERATED_BODY()
};

class QUESTSYSTEM_API IKillableActor
{
	GENERATED_BODY()

public:

	FOnKilled FOnKilled;

	virtual void KilledActor(AActor* DestroyedPawn, AActor* Killer);
};
