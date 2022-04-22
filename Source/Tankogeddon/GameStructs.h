#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
// ReSharper disable once CppUESourceFileWithoutPredefinedMacros
#include "GameStructs.generated.h"


UENUM(BlueprintType)
enum class ECannonType : uint8 {
FireProjectile = 0 UMETA(DisplayName = "Use projectile"),
FireTrace = 1 UMETA(DisplayName = "Use trace"),
FireSpecial = 0 UMETA(DisplayName = "Use special")
};
