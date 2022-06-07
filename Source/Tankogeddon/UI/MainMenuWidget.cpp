
#include "MainMenuWidget.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"


void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();


	if(NewGameBtn)
	{
		NewGameBtn->OnClicked.AddDynamic(this, &UMainMenuWidget::OnNewGameClicked);
	}

	if(LoadBtn)
	{
		LoadBtn->OnClicked.AddDynamic(this, &UMainMenuWidget::OnLoadClicked);
	}

	if(OptionBtn)
	{
		OptionBtn->OnClicked.AddDynamic(this, &UMainMenuWidget::OnOptionClicked);
	}

	if(QuitBtn)
	{
		QuitBtn->OnClicked.AddDynamic(this, &UMainMenuWidget::OnQuitClicked);
	}

	

}

void UMainMenuWidget::OnNewGameClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), "Level1");
}

void UMainMenuWidget::OnLoadClicked()
{
}

void UMainMenuWidget::OnOptionClicked()
{
}

void UMainMenuWidget::OnQuitClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}
