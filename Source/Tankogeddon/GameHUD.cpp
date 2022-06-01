// Fill out your copyright notice in the Description page of Project Settings.


#include "GameHUD.h"

#include "Blueprint/WidgetBlueprintLibrary.h"

void AGameHUD::BeginPlay()
{
	Super::BeginPlay();

	const UWorld* World = GetWorld();
	if (World())
	{
		APlayerController* PC = GetWorld()->GetFirstPlayerController();

		if (PC)
        {
			UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PC, nullptr, EMouseLockMode::DoNotLock, false);
			PC->bShowMouseCursor = true;
        }
    }

	UseWidget(EWidgetID::MainMenu, true, 0);
}

UUserWidget* AGameHUD::GetCurrentWidget()
{
	return CurrentWidget;
}

void AGameHUD::RemoveCurrentWidgetFromViewport()
{
	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
		CurrentWidget = nullptr;
		CurrentWidgetID = EWidgetID::None;
	}
}

UUserWidget* AGameHUD::AddWidgetToViewportByClass(TSubclassOf<UUserWidget> WidgetClass, int32 ZOrder /*= 0*/)
{
	CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
	if (CurrentWidget)
	{
		CurrentWidget->AddToViewport(ZOrder);
	}
	return CurrentWidget;
}

UUserWidget* AGameHUD::UseWidget(EWidgetID widgetID, bool RemovePrevious,
int32 ZOrder)
{
	if (CurrentWidgetID == widgetID)
		return CurrentWidget;

	if (RemovePrevious)
	{
		RemoveCurrentWidgetFromViewport();
	}

	TSubclassOf<UUserWidget> WidgetClassToUse = WidgetClases.FindRef(widgetID);

	if (WidgetClassToUse.Get())
	{
		CurrentWidgetID = widgetID;
		return AddWidgetToViewportByClass(WidgetClassToUse, ZOrder);
	};

	return nullptr;
}
