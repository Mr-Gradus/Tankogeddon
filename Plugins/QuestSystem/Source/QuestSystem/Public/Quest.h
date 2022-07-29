#pragma once

#include "CoreMinimal.h"
#include "Objective.h"
#include "GameFramework/Actor.h"
#include "Quest.generated.h"

class AQuest;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnQuestStatusChanged, AQuest*);

DECLARE_MULTICAST_DELEGATE(FOnQuestStatusChangedNoParams);

UCLASS()
class QUESTSYSTEM_API AQuest : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AQuest();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	FOnQuestStatusChanged OnQuestStatusChanged;

	FOnQuestStatusChangedNoParams OnQuestStatusChangedNoParams;

	UPROPERTY(EditAnywhere)
	FText Name;

	UPROPERTY(EditAnywhere)
	FText Description;

	UPROPERTY(EditAnywhere)
	TArray<UObjective*> Objectives;

	UPROPERTY(EditAnywhere)
	bool bIsStoryQuest = true;

	UPROPERTY(EditAnywhere)
	bool bKeepObjectivesOrder = true;

	UPROPERTY(EditAnywhere)
	AQuest* PreviousQuest;

	UPROPERTY(EditAnywhere)
	bool bIsTaken = false;

	UFUNCTION(BlueprintCallable, CallInEditor)
	void UpdateLocation();

	UFUNCTION()
	void TakeQuest(AActor* Character);

	UFUNCTION()
	TArray<AActor*> GetObjectiveComponents();

	void OnObjectiveCompleted(UObjective* Objective);

	bool IsCompleted();

	bool IsStoryQuest();

	FText& GetQuestName();

	bool QuestStatus = false;	

	TArray<UObjective*>& GetObjectives();

	UFUNCTION(BlueprintCallable, CallInEditor)
	void AddInteractObjective();
	
	UFUNCTION(BlueprintCallable, CallInEditor)
	void AddLocationObjective();

	UFUNCTION(BlueprintCallable, CallInEditor)
	void AddCollectibleObjective();

	UFUNCTION(BlueprintCallable, CallInEditor)
	void AddKillableObjective();
};
