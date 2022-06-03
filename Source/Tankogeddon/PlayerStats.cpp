
#include "PlayerStats.h"

void UPlayerStats::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPlayerStats::ChangeHealth(float CurrentHealth, float MaxHealth, bool bAnimation)
{
	const auto Percent{ CurrentHealth / MaxHealth };
	PlayerHealth->SetPercent(Percent);

//	if (bAnimation && PlayerTakenDamageAnimation)
//	{
//		PlayAnimation(PlayerTakenDamageAnimation);
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