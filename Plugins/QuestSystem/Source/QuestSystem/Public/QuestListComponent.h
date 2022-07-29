#pragma once

#include "CoreMinimal.h"
#include "CurrentObjectives.h"
#include "Quest.h"
#include "Components/ActorComponent.h"
#include "QuestListComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QUESTSYSTEM_API UQuestListComponent : public UActorComponent
{
	GENERATED_BODY()

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnQuestAction, AQuest*);

public:	
	
	UQuestListComponent();

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AddQuest(AQuest* Quest);
	
	const TArray<AQuest*>& GetQuests();
	
	AQuest* GetActiveQuest() const;
	
	void SetActiveQuest(AQuest* Quest);
	
	FOnQuestAction OnActiveQuestChanged;

	UPROPERTY()
	TArray<AQuest*> AcceptedQuests;
	
	UPROPERTY()
	AQuest* ActiveQuest;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCurrentObjectives> CurrentObjectivesWidget;
};
