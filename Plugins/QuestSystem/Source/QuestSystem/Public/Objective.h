#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Objective.generated.h"

UENUM()
enum class EObjectiveType: uint8
{
	Location,
	Interact,
	Collect,
	Kill
};

class UObjective;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnObjectiveCompleted, UObjective* /*Objective*/);

UCLASS(Abstract)
class QUESTSYSTEM_API UObjective : public UObject
{
	GENERATED_BODY()
	
public:

	virtual void ActivateObjective(AActor* Instigator){}

	virtual AActor* GetInteractiveComponent() const
	{
		return nullptr;
	};

	UPROPERTY(EditAnywhere)
	FText Description;

	UPROPERTY(VisibleAnywhere)
	EObjectiveType Type;

	FOnObjectiveCompleted OnObjectiveCompleted;

	UPROPERTY(VisibleAnywhere)
	bool bIsCompleted;

	UPROPERTY(VisibleAnywhere)
	bool bCanBeCompleted;
};

//============================================ INTERACTION ============================================

UCLASS() class QUESTSYSTEM_API UInteractionObjective : public UObjective
{
	GENERATED_BODY()
	
public:

	UInteractionObjective();

	virtual void ActivateObjective(AActor* Instigator) override;

	UPROPERTY(EditAnywhere, meta = (AllowedClasses ="InteractableObject"))
	AActor* Target;	
};

//============================================ LOCATION ============================================

UCLASS()
class QUESTSYSTEM_API ULocationObjective : public UObjective
{
	GENERATED_BODY()
	
public:
	
	ULocationObjective();

	virtual void ActivateObjective(AActor* Instigator) override;

	UPROPERTY(EditAnywhere, meta = (AllowedClasses = "LocationMarker"))
    AActor* Marker;
};

//============================================ COLLECTIBLE ============================================

UCLASS()
class QUESTSYSTEM_API UCollectibleObjective : public UObjective
{
	GENERATED_BODY()
	
public:
	
	UCollectibleObjective();

	virtual void ActivateObjective(AActor* Instigator) override;

	UPROPERTY(EditAnywhere, meta = (AllowedClasses = "AmmoBox, CannonBox"))
	AActor* CollectibleObject;
};

//============================================ KILLABLE ============================================

UCLASS()
class QUESTSYSTEM_API UKillableObjective : public UObjective
{
	GENERATED_BODY()
	
public:
	
	UKillableObjective();

	virtual void ActivateObjective(AActor* Instigator) override;

	UPROPERTY(EditAnywhere, meta = (AllowedClasses = "ParentTankTurret, TankFactory"))
	AActor* KillableObject;
};