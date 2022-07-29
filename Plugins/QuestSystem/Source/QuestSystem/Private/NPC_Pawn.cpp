#include "NPC_Pawn.h"

#include "QuestListComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"

ANPC_Pawn::ANPC_Pawn()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* ScnComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = ScnComp;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));	
	Body->SetupAttachment(RootComponent);
	
	Head = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Head"));
	Head->SetupAttachment(Body);	

	NotificationWidget = CreateDefaultSubobject<UWidgetComponent>("NotificationWidget");
	NotificationWidget->SetWidgetClass(NotificationWidgetClass);
	NotificationWidget->SetupAttachment(RootComponent);
	NotificationWidget->SetHiddenInGame(true);
}

void ANPC_Pawn::BeginPlay()
{
	Super::BeginPlay();
	
	// TArray<AActor*> AttachedActors;	
	// GetAttachedActors(AttachedActors);
	//
	// for (auto& Actor: AttachedActors)
	// {
	// 	AQuest* Quest = Cast<AQuest>(Actor);
	//
	// 	if (Quest->Objectives.Num() <= 0)
	// 	{
	// 		SetHiddenNotificationWidget(true);
	// 	}		
	// }
}

void ANPC_Pawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
}

void ANPC_Pawn::Interact_Implementation(AActor* InteractInstigator)
{
	if (InteractInstigator == nullptr)
	{
		return;
	}		

	UQuestListComponent* QuestList = Cast<UQuestListComponent>(InteractInstigator->GetComponentByClass(UQuestListComponent::StaticClass()));
	
	if (QuestList == nullptr)
	{
		return;
	}

	TArray<AActor*> AttachedActors;
	
	GetAttachedActors(AttachedActors);
	
	for (auto& Actor: AttachedActors)
	{
		AQuest* Quest = Cast<AQuest>(Actor);
		
		Quest->OnQuestStatusChangedNoParams.AddUObject(this, &ThisClass::GetAcceptableQuest);

		if (Quest == nullptr || Quest->Objectives.Num() <= 0)
		{
			continue;
		}

		if (Quest->bIsTaken || (Quest->PreviousQuest && !Quest->PreviousQuest->IsCompleted()))
		{
			continue;
		}
		
		if (QuestDialogClass)
		{
			UQuestDialog* QuestDialog = CreateWidget<UQuestDialog>(GetWorld(), QuestDialogClass);
			
			QuestDialog->Init(Quest);

			QuestDialog->OnQuestAccepted.AddUObject(QuestList, &UQuestListComponent::AddQuest, Quest);

			QuestDialog->OnQuestAccepted.AddUObject(this, &ThisClass::SetHiddenNotificationWidget, true);
			
			QuestDialog->OnDialogExit.BindLambda([this, InteractInstigator]()
			{
				NotifyInteractionFinished(this, InteractInstigator);
			});
			
			QuestDialog->AddToViewport();
			
			return;
		}
	}
	
	NotifyInteractionFinished(this, InteractInstigator);
}

const TArray<AQuest*> ANPC_Pawn::GetQuests()
{
	TArray<AActor*> AttachedActors;
	TArray<AQuest*> QuestList;
	GetAttachedActors(AttachedActors);
	for (auto& Actor : AttachedActors)
	{
		AQuest* Quest = Cast<AQuest>(Actor);
		if (Quest == nullptr)
		{
			continue;
		};

		QuestList.Add(Quest);
	}

	return QuestList;
}

TArray<AActor*> ANPC_Pawn::GetAllObjectiveComps()
{
	TArray<AActor*> ObjectiveComponents;
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);
	for (auto& Actor : AttachedActors)
	{
		AQuest* Quest = Cast<AQuest>(Actor);
		if (Quest == nullptr)
		{
			continue;
		};

		ObjectiveComponents.Append(Quest->GetObjectiveComponents());
	}

	return ObjectiveComponents;
}

bool ANPC_Pawn::HasQuests()
{
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);
	return (AttachedActors.Num() > 0);
}

void ANPC_Pawn::GetAcceptableQuest()
{
	TArray<AActor*> AttachedActors;
	
	GetAttachedActors(AttachedActors);

	bool bAcceptableQuest = false;
	
	for (auto& Actor: AttachedActors)
	{
		AQuest* Quest = Cast<AQuest>(Actor);

		if (Quest == nullptr)
		{
			continue;
		}

		if (Quest->bIsTaken || (Quest->PreviousQuest && !Quest->PreviousQuest->IsCompleted()))
		{
			continue;
		}

		bAcceptableQuest = true;
	}

	SetHiddenNotificationWidget(!bAcceptableQuest);
}

void ANPC_Pawn::SetHiddenNotificationWidget(bool bSetHidden)
{
	if (NotificationWidget)
	{
		NotificationWidget->SetHiddenInGame(bSetHidden);
	}
}