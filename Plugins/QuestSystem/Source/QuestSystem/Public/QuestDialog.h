#pragma once

#include "CoreMinimal.h"
#include "QuestDescription.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "QuestDialog.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnQuestAccepted);

UCLASS()
class QUESTSYSTEM_API UQuestDialog : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	virtual void NativeConstruct() override;	
	
	FOnQuestAccepted OnQuestAccepted;

	FSimpleDelegate OnDialogExit;

	UPROPERTY(meta = (BindWidget))
	UQuestDescription* Description;

	UPROPERTY(meta = (BindWidget))
	UButton* AcceptButton;

	UPROPERTY(meta = (BindWidget))
	UButton* RejectButton;

	void Init(AQuest* Quest);	

	UFUNCTION()
	void OnAccepted();

	UFUNCTION()
	void OnRejected();

	void HideDialog();
};
