#include "PlayerStats.h"

#include "StyleSet.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Tankogeddon/TankPawn.h"
#include "Kismet/KismetTextLibrary.h"
#include "Tankogeddon/ParentTankTurret.h"

void UPlayerStats::NativeConstruct()
{
	Super::NativeConstruct();

	
	if(QuitBtn)
	{
		QuitBtn->OnClicked.AddDynamic(this, &UPlayerStats::OnQuitClicked);
	}
	
	if (QuitBtn)
	{
		QuitBtn->WidgetStyle = FStyleSet::Get().GetWidgetStyle<FButtonStyle>(FName("ButtonStyle")
			);
	}	
}

void UPlayerStats::NativePreConstruct()
{

	Super::NativePreConstruct();
	
	if (QuitBtn)
	{
		QuitBtn->WidgetStyle = FStyleSet::Get().GetWidgetStyle<FButtonStyle>(FName("ButtonStyle")
			);
	}	
}

void UPlayerStats::OnQuitClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);

}

void UPlayerStats::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	const AParentTankTurret* Parent = Cast<AParentTankTurret>(GetOwningPlayerPawn()); 

	if (Parent)
	{
		float Ammo = Cast<AParentTankTurret>(GetOwningPlayerPawn())->GetCannon()->GetAmmo();
		
		PlayerAmmo->SetText(UKismetTextLibrary::Conv_FloatToText(Ammo,ERoundingMode::HalfFromZero));

		float HP = Cast<AParentTankTurret>(GetOwningPlayerPawn())->GetHealthComponent()->GetHealthPercent();
		
		PlayerHealth->SetPercent(HP);

		ACannon* CannonInfo = Cast<ATankPawn>(GetOwningPlayerPawn())->GetCannon();
		PlayerCannon->SetText(FText::FromString(CannonInfo->GetName()));
	}
	
	

/*
	if (Cast<AParentTankTurret>(GetOwningPlayerPawn()))
	{
		float HP = Cast<AParentTankTurret>(GetOwningPlayerPawn())->GetHealthComponent()->GetHealthPercent();
		
		PlayerHealth->SetPercent(HP);
	}

	if (Cast<ATankPawn>(GetOwningPlayerPawn()))
	{
		ATankPawn* CannonInfo = Cast<ATankPawn>(GetOwningPlayerPawn());
		PlayerCannon->SetText(FText::FromString(CannonInfo->GetName()));
	}
*/
	
}

//	if (bAnimation && PlayerTakenDamageAnim)
//	{
//		PlayAnimation(PlayerTakenDamageAnim);
//	}
