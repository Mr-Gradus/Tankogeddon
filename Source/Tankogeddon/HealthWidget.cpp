#include "HealthWidget.h"
#include "Components/WidgetComponent.h"

void UHealthWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UWidgetComponent * comp = CreateDefaultSubobject<UWidgetComponent>(TEXT("BarHP"));
	comp->SetWidgetClass(UHealthWidget::StaticClass());
	UHealthWidget * myProgress = Cast<UHealthWidget>(comp->GetUserWidgetObject());
	myProgress->SetHealthValue(1.f);
}


void UHealthWidget::SetHealthValue(float Health)
{
	TankPawnProgressBar->SetPercent(Health);
}
