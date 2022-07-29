#include "QuestListComponent.h"

UQuestListComponent::UQuestListComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UQuestListComponent::BeginPlay()
{
	Super::BeginPlay();

	if (CurrentObjectivesWidget)
	{
		UCurrentObjectives* ObjectivesWidget = CreateWidget<UCurrentObjectives>(GetWorld(), CurrentObjectivesWidget);
		
		OnActiveQuestChanged.AddUObject(ObjectivesWidget, &UCurrentObjectives::SetCurrentObjectives);
		
		ObjectivesWidget->AddToViewport();
	}
}

void UQuestListComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UQuestListComponent::AddQuest(AQuest* Quest)
{
	if (Quest == nullptr)
	{
		return;
	}		
	
	if (ActiveQuest == nullptr || ActiveQuest->IsCompleted())
	{
		ActiveQuest = Quest;
		
		OnActiveQuestChanged.Broadcast(ActiveQuest);		
	}
	
	AcceptedQuests.Add(Quest);
	
	Quest->TakeQuest(GetOwner());

	Quest->OnQuestStatusChanged.AddLambda([this](AQuest* ChangedQuest)
	{
		if (ActiveQuest == ChangedQuest && OnActiveQuestChanged.IsBound())
		{
			OnActiveQuestChanged.Broadcast(ChangedQuest);
		}		
	});
}

const TArray<AQuest*>& UQuestListComponent::GetQuests()
{
	return AcceptedQuests;
}

AQuest* UQuestListComponent::GetActiveQuest() const
{
	return ActiveQuest;
}

void UQuestListComponent::SetActiveQuest(AQuest* Quest)
{
	if (AcceptedQuests.Contains(Quest))
	{
		ActiveQuest = Quest;
		
		if (OnActiveQuestChanged.IsBound())
		{
			OnActiveQuestChanged.Broadcast(ActiveQuest);
		}
	}
}

