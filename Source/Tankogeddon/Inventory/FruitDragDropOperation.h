#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "FruitDragDropOperation.generated.h"


UCLASS()
class TANKOGEDDON_API UFruitDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:
	FText FruitName;
};
