#include "HealthWidget.h"

#include "Components/WidgetComponent.h"

void UHealthWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UHealthWidget::SetHealthValue(float Health) const
{
	TankPawnProgressBar->SetPercent(Health);
}
