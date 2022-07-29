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

	UFUNCTION(BlueprintPure, meta = (WorldContext = WorldContextObject))
	static USaveGameManager* GetSaveManager(const UObject* WorldContextObject);

protected:

	UPROPERTY()
	USaveGameManager* SaveGameManager;
};
