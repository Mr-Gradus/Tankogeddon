#include "Tankogeddon/Inventory/ActorSpawnerWidget.h"

#include "Kismet/GameplayStatics.h"
#include "Tankogeddon/Setting/TankPlayerController.h"


void UActorSpawnerWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	ATankPlayerController* PC = Cast<ATankPlayerController>(PlayerController);
	PC->OnMouseButtonUp.AddUObject(this, &UActorSpawnerWidget::OnMouseButtonUp);
}

FReply UActorSpawnerWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		DraggedActor = GetWorld()->SpawnActor<AActor>(DraggedActorClass);
	}
	return FReply::Handled();
}

void UActorSpawnerWidget::OnMouseButtonUp()
{
	if (DraggedActor)
	{
		DraggedActor = nullptr;
	}
}


void UActorSpawnerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (DraggedActor && PlayerController)
	{
		FVector WorldMouseLocation;

		FVector WorldMouseDirection;
		
		PlayerController->DeprojectMousePositionToWorld(WorldMouseLocation, WorldMouseDirection);

		FHitResult HitResult;

		float TraceDistance = 1000000.f;

		GetWorld()->LineTraceSingleByChannel(HitResult, WorldMouseLocation, WorldMouseLocation +
			WorldMouseDirection * TraceDistance, ECollisionChannel::ECC_WorldStatic);

		
		
		if (HitResult.Actor.Get())
		{
			DraggedActor->SetActorLocation(HitResult.Location);
		}
	}
}
