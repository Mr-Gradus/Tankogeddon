#include "InventoryCharacter.h"


AInventoryCharacter::AInventoryCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("Inventory");
	InventoryManagerComponent = CreateDefaultSubobject<UInventoryManagerComponent>("InventoryManager");
}

void AInventoryCharacter::BeginPlay()
{
	Super::BeginPlay();

	InventoryManagerComponent->Init(InventoryComponent);
}

void AInventoryCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInventoryCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

