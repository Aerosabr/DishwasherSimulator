#include "World/Appliance/DishRack.h"

#include "Data/ItemDataStructs.h"
#include "InteractionSystem/DSCharacter.h"
#include "Items/Dish.h"
#include "Manager/DSManager.h"

class ADish;

ADishRack::ADishRack()
{
	PrimaryActorTick.bCanEverTick = true;

	RackMesh = CreateDefaultSubobject<UStaticMeshComponent>("RackMesh");
	SetRootComponent(RackMesh);
	
}

void ADishRack::BeginPlay()
{
	Super::BeginPlay();

	InteractableData = InstanceInteractableData;
}

void ADishRack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADishRack::BeginFocus()
{
	IInteractionInterface::BeginFocus();

	if (RackMesh)
		RackMesh->SetRenderCustomDepth(true);
}

void ADishRack::EndFocus()
{
	IInteractionInterface::EndFocus();
	
	if (RackMesh)
		RackMesh->SetRenderCustomDepth(false);
}

void ADishRack::Interact(ADSCharacter* PlayerCharacter)
{
	IInteractionInterface::Interact(PlayerCharacter);

	UE_LOG(LogTemp, Warning, TEXT("Interacting with Dish Rack"));

	if(PlayerCharacter->GetHeldItemType() == EItemType::Dish)
	{
		if (ADish* TempDish = Cast<ADish>(PlayerCharacter->HeldItem); TempDish->GetDishState() == EDishState::Sanitized)
		{
			PlayerCharacter->HeldItem->Destroy();
			PlayerCharacter->HeldItem = nullptr;
			PlayerCharacter->SetIsHoldingItem(false, EItemType::None);
			if (UDSManager* GameInstance = Cast<UDSManager>(GetGameInstance()))
				GameInstance->ChangeMoney(1);
		}
	}
}

bool ADishRack::CanInteract()
{
	return bCanInteract;
}



