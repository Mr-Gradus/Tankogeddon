#pragma once

#include "CoreMinimal.h"
#include "TargetController.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

class ATankPawn;


UCLASS()
class TANKOGEDDON_API ATankPlayerController : public APlayerController, public ITargetController
{
	GENERATED_BODY()

protected:
	
	UPROPERTY()
	ATankPawn* TankPawn;

	UPROPERTY()
	FVector MousePos;

public:
	ATankPlayerController(const FObjectInitializer & Obj);
	
	virtual void SetupInputComponent() override;

	virtual void Tick(float DeltaTime) override;
	
	virtual FVector GetTargetLocation() const override;

	void Fire();

	void FireSpecial();

	void ChangeCannon();

	FSimpleMulticastDelegate OnMouseButtonUp;


protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

	void MoveForward(float AxisValue);

	void RotateRight(float AxisValue);

	void GRotateRight(float AxisValue);

	void OnLeftMouseButtonUp() const;
};
