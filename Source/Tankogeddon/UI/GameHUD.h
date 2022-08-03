#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameHUD.generated.h"

UENUM()
enum class EWidgetID : uint8
{
None,
MainMenu,
GameOver,
PlayerStats,
SaveLoad	

};

UCLASS()
class TANKOGEDDON_API AGameHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere)
	TMap<EWidgetID, TSubclassOf<UUserWidget>> WidgetClasses;

	UPROPERTY()
	UUserWidget * CurrentWidget;
	
	EWidgetID CurrentWidgetID;

public:
	virtual void BeginPlay() override;
	
	UUserWidget * UseWidget(EWidgetID widgetID, bool RemovePrevious = true, int32 ZOrder = 0);

	UUserWidget* GetCurrentWidget();

	void RemoveCurrentWidgetFromViewport();

	UUserWidget * AddWidgetToViewportByClass(TSubclassOf<UUserWidget> WidgetClass, int32 ZOrder = 0);
};
