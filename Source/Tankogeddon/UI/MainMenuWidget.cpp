
#include "MainMenuWidget.h"

#include "ButtonsWidgetStyle.h"
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
	if (QuitBtn)
	{
		QuitBtn->WidgetStyle = FStyleSet::Get().GetWidgetStyle<FButtonStyle>(FName("ButtonStyle")
			);
	}	
}

void UMainMenuWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (RadioButton)
	{
		SelectTheme();
	}
}

void UMainMenuWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (RadioButton)
	{
		SelectTheme();
	}

	if (QuitBtn)
	{
		QuitBtn->WidgetStyle = FStyleSet::Get().GetWidgetStyle<FButtonStyle>(FName("ButtonStyle")
			);
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

void UMainMenuWidget::SelectTheme()
{

const auto Theme = ThemeComboBox->GetSelectedIndex(); 
	if (Theme == 1)
	{
		RadioButton->WidgetStyle = FStyleSet::Get().GetWidgetStyle<FRadioButtonsStyle>(FName("RadioButtonsStyleBlack"));
	}
	if (Theme == 2)
	{
		RadioButton->WidgetStyle = FStyleSet::Get().GetWidgetStyle<FRadioButtonsStyle>(FName("RadioButtonsStyleRed"));	
	}
	if (Theme == 0)
	{
		RadioButton->WidgetStyle = FStyleSet::Get().GetWidgetStyle<FRadioButtonsStyle>(FName("RadioButtonsStyle"));
	}
}

