#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"
#include "InventoryManagerComponent.h"
#include "GameFramework/Character.h"
#include "InventoryCharacter.generated.h"

UCLASS()
class MYINVENTORY_API AInventoryCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AInventoryCharacter();

	UPROPERTY(EditDefaultsOnly)
	UInventoryComponent * InventoryComponent;
	
	UPROPERTY(EditDefaultsOnly)
	UInventoryManagerComponent * InventoryManagerComponent;
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
