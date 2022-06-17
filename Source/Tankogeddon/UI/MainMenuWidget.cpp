
#include "MainMenuWidget.h"

#include "StyleSet.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Tankogeddon/UI/SRadioButtonsList.h"

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

void UMainMenuWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (RadioButtons_0)
	{
		RadioButtons_0->WidgetStyle = FStyleSet::Get().GetWidgetStyle<FRadioButtonsStyle>(FName("RadioButtonsStyle")
			);
	}

//	if (QuitBtn)
//	{
//		QuitBtn->WidgetStyle = FStyleSet::Get().GetWidgetStyle<FButtonsStyle>(FName("ButtonsStyle");
//	}	
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

