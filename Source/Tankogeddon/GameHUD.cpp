
#include "GameHUD.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void AGameHUD::BeginPlay()
{
	Super::BeginPlay();

	const UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PC = World->GetFirstPlayerController();

		if (PC)
        {
			UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(PC, nullptr, EMouseLockMode::DoNotLock, false);
			PC->bShowMouseCursor = true;
        }
    }

	if (GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		UseWidget(EWidgetID::PlayerStats);
	}
	else
	{
		UseWidget(EWidgetID::MainMenu);
	}
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

UUserWidget* AGameHUD::AddWidgetToViewportByClass(TSubclassOf<UUserWidget> WidgetClass, int32 ZOrder)
{
	RemoveCurrentWidgetFromViewport();
	
	CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
	if (CurrentWidget)
	{
		CurrentWidget->AddToViewport(ZOrder);
	}
	return CurrentWidget;
}

UUserWidget* AGameHUD::UseWidget(EWidgetID widgetID, bool RemovePrevious, int32 ZOrder)
{
	if (CurrentWidgetID == widgetID)
		return CurrentWidget;

	if (RemovePrevious)
	{
		RemoveCurrentWidgetFromViewport();
	}

	TSubclassOf<UUserWidget> WidgetClassToUse = WidgetClasses.FindRef(widgetID);

	if (WidgetClassToUse.Get())
	{
		auto ResultWidget{ AddWidgetToViewportByClass(WidgetClassToUse, ZOrder) };
		CurrentWidgetID = widgetID;
		return ResultWidget;
	};

	return nullptr;
}
