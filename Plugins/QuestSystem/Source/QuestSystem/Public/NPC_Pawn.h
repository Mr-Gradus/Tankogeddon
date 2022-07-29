#pragma once

#include "CoreMinimal.h"
#include "InteractableObject.h"
#include "NotificationWidget.h"
#include "QuestDialog.h"
#include "GameFramework/Pawn.h"
#include "NPC_Pawn.generated.h"

UCLASS()
class QUESTSYSTEM_API ANPC_Pawn : public APawn, public IInteractableObject
{
	GENERATED_BODY()

public:
	
	ANPC_Pawn();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void Interact_Implementation(AActor* InteractInstigator) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Components")
	UStaticMeshComponent* Body;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Components")
	UStaticMeshComponent* Head;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UQuestDialog> QuestDialogClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	TSubclassOf<UNotificationWidget> NotificationWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UWidgetComponent* NotificationWidget;

	const TArray<AQuest*> GetQuests();

	TArray<AActor*> GetAllObjectiveComps();

	bool HasQuests();

	UFUNCTION()
	void GetAcceptableQuest();

	UFUNCTION()
	void SetHiddenNotificationWidget(bool bSetHidden);
};

