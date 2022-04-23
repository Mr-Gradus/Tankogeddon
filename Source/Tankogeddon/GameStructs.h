#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "GameStructs.generated.h"


UENUM(BlueprintType)
enum class ECannonType : uint8 {
	FireProjectile = 0 UMETA(DisplayName = "Use projectile"),
	FireTrace = 1 UMETA(DisplayName = "Use trace"),
	FireSpecial = 0 UMETA(DisplayName = "Use special"),
	FirePlasma = 0 UMETA(DispalayName = "Use plasma")
};

USTRUCT()
struct FDamageData
{
	GENERATED_BODY()

	UPROPERTY()
	float DamageValue;
	UPROPERTY()
	AActor * Instigator;
	UPROPERTY()
	AActor * DamageMaker;
};