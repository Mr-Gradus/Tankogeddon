
#include "PlayerStats.h"

#include "ParentTankTurret.h"
#include "TankPawn.h"
#include "Kismet/KismetTextLibrary.h"

void UPlayerStats::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPlayerStats::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (Cast<AParentTankTurret>(GetOwningPlayerPawn()))
	{
		float Ammo = Cast<AParentTankTurret>(GetOwningPlayerPawn())->GetCannon()->GetAmmo();
		//TSubclassOf<ACannon> CannonName = Cast<AParentTankTurret>(GetOwningPlayerPawn())->GetCannon()->;
		
		PlayerAmmo->SetText(UKismetTextLibrary::Conv_FloatToText(Ammo,ERoundingMode::HalfFromZero));
		//PlayerCannon->SetText(FText::FromString(CannonName));
	}
		
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

	
}

//void UPlayerStats::CannonInfo(const FString& CannonClass)
//{
//	PlayerCannon->SetText(FText::FromString(CannonClass));
//}

/*
void UPlayerStats::ChangeHealth(float CurrentHealth, float MaxHealth, bool bAnimation)
{
	const auto Percent{ CurrentHealth / MaxHealth };
	PlayerHealth->SetPercent(Percent);

//	if (bAnimation && PlayerTakenDamageAnim)
//	{
//		PlayAnimation(PlayerTakenDamageAnim);
//	}

}



void UPlayerStats::ChangeAmmo(float CurrentAmmo, float MaxAmmo)
{
	if (PlayerAmmo)
	{
		PlayerAmmo->SetText(FText::Format(FText::FromString("{0}/{1}"), CurrentAmmo, MaxAmmo));
	}
}
*/
