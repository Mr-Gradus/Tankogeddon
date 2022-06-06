#include "HealthWidget.h"

#include "ParentTankTurret.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetTextLibrary.h"

void UHealthWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UHealthWidget::SetHealthValue(float Health) const
{
	TankPawnProgressBar->SetPercent(Health);
}
