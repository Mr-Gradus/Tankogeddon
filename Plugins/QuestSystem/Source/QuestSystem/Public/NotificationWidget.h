#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "NotificationWidget.generated.h"

UCLASS()
class QUESTSYSTEM_API UNotificationWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(meta = (BindWidget))
	UImage* NoticeImage;
};
