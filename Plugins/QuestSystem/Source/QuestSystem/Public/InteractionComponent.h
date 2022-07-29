#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "InteractionComponent.generated.h"

UCLASS(ClassGroup = "Collision", meta = (DisplayName = "Interaction Collision",BlueprintSpawnableComponent))
class QUESTSYSTEM_API UInteractionComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> InteractMessageWidget;

	UPROPERTY()
	UUserWidget* InteractMessage;

	UPROPERTY()
	AActor* ActorToInteract;

	UFUNCTION()
	void OnComponentBeginOverlapFunc(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnComponentEndOverlapFunc(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void Interact();
};
