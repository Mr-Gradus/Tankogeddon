
#include "PlayerStats.h"

#include "ParentTankTurret.h"
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
		TSubclassOf<ACannon> CannonName = Cast<AParentTankTurret>(GetOwningPlayerPawn())->CurrentCannon;
		
		PlayerAmmo->SetText(UKismetTextLibrary::Conv_FloatToText(Ammo,ERoundingMode::HalfFromZero));
	}
		
	if (Cast<AParentTankTurret>(GetOwningPlayerPawn()))
	{
		float HP = Cast<AParentTankTurret>(GetOwningPlayerPawn())->GetHealthComponent()->GetHealthPercent();
		
		PlayerHealth->SetPercent(HP);
	}
}


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

void UPlayerStats::ChangeCannon(const FString& CannonClass)
{
	PlayerCannon->SetText(FText::FromString(CannonClass));
}

void UPlayerStats::ChangeAmmo(float CurrentAmmo, float MaxAmmo)
{
	if (PlayerAmmo)
	{
		PlayerAmmo->SetText(FText::Format(FText::FromString("{0}/{1}"), CurrentAmmo, MaxAmmo));
	}
}
*/
