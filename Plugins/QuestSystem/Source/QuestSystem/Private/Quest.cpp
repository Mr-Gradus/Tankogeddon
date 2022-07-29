#include "Quest.h"
#include "Objective.h"

AQuest::AQuest()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AQuest::BeginPlay()
{
	Super::BeginPlay();

	AActor* ParentActor = GetAttachParentActor();
}

void AQuest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AQuest::UpdateLocation()
{
	AActor* ParentActor = GetAttachParentActor();

	if (ParentActor)
	{
		SetActorLocation(ParentActor->GetActorLocation());
	}
}

void AQuest::TakeQuest(AActor* Character)
{
	if (bIsTaken)
	{
		return;
	}

	for (int32 i = 0; i < Objectives.Num(); i++)
	{
		UObjective* Objective = Objectives[i];
		
		if (Objective == nullptr)
		{
			continue;
		}

		Objective->ActivateObjective(Character);
		
		Objective->bCanBeCompleted = i == 0 || !bKeepObjectivesOrder;
		
		Objective->OnObjectiveCompleted.AddUObject(this, &ThisClass::OnObjectiveCompleted);
	}
	
	bIsTaken = true;
}

TArray<AActor*> AQuest::GetObjectiveComponents()
{
	TArray<AActor*> ObjCollection;
	for (auto* ThisObjective : Objectives)
	{
		ObjCollection.Add(ThisObjective->GetInteractiveComponent());
	}

	return ObjCollection;
}

void AQuest::OnObjectiveCompleted(UObjective* Objective)
{
	if (bKeepObjectivesOrder)
	{
		int32 Index;
		
		if (Objectives.Find(Objective, Index) && Objectives.IsValidIndex(Index + 1))
		{
			Objectives[Index+1]->bCanBeCompleted = true;
		}
	}

	for (int32 i = 0; i < Objectives.Num(); i++)
	{
		UObjective* Object = Objectives[i];

		if (!Object->bIsCompleted)
		{
			QuestStatus = false;
			
			return;
		}
	}

	QuestStatus = true;

	if (OnQuestStatusChanged.IsBound())
	{
		OnQuestStatusChanged.Broadcast(this);
	}

	if (OnQuestStatusChangedNoParams.IsBound())
	{
		OnQuestStatusChangedNoParams.Broadcast();
	}	
}

bool AQuest::IsCompleted()
{
	return QuestStatus;
}

bool AQuest::IsStoryQuest()
{
	return bIsStoryQuest;
}

FText& AQuest::GetQuestName()
{
	return Name;
}

TArray<UObjective*>& AQuest::GetObjectives()
{
	return Objectives;
}

void AQuest::AddInteractObjective()
{
	Objectives.Add(NewObject<UInteractionObjective>(this));
}

void AQuest::AddLocationObjective()
{
	Objectives.Add(NewObject<ULocationObjective>(this));
}

void AQuest::AddCollectibleObjective()
{
	Objectives.Add(NewObject<UCollectibleObjective>(this));
}

void AQuest::AddKillableObjective()
{
	Objectives.Add(NewObject<UKillableObjective>(this));
}
