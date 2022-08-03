#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

class USaveGameManager;

UCLASS()
class TANKOGEDDON_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

protected:
	UPROPERTY(BlueprintReadOnly)
	USaveGameManager* SaveGameManager;
};
