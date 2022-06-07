#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "GameStructs.generated.h"


UENUM(BlueprintType)
enum class ECannonType : uint8 {
	FireProjectile = 0 UMETA(DisplayName = "Use projectile"),
	FireTrace = 1 UMETA(DisplayName = "Use trace"),
	
};

USTRUCT(BlueprintType)
struct FDamageInfo
{
	GENERATED_BODY()

public:
	UPROPERTY()
	AActor* Instigator = nullptr;
	UPROPERTY()
	AActor* DamageMaker;
	UPROPERTY()
	float Damage;
};
