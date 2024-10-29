#include "World/Appliance/Rinser.h"

#include "Data/ItemDataStructs.h"
#include "InteractionSystem/DSCharacter.h"
#include "Items/Dish.h"

ARinser::ARinser()
{
	PrimaryActorTick.bCanEverTick = true;
	
	RinserMesh = CreateDefaultSubobject<UStaticMeshComponent>("RinserMesh");
	SetRootComponent(RinserMesh);
}

void ARinser::BeginPlay()
{
	Super::BeginPlay();
	
	InteractableData = InstanceInteractableData;
}

void ARinser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARinser::BeginFocus()
{
	if (RinserMesh)
		RinserMesh->SetRenderCustomDepth(true);
}

void ARinser::EndFocus()
{
	if (RinserMesh)
		RinserMesh->SetRenderCustomDepth(false);
}

void ARinser::BeginInteract()
{

}

void ARinser::EndInteract()
{

}

void ARinser::Interact(ADSCharacter* PlayerCharacter)
{
	UE_LOG(LogTemp, Warning, TEXT("Interacting with Rinser"));
	switch(PlayerCharacter->GetHeldItemType())
	{
		case EItemType::None:
			
			break;
		case EItemType::Dish:
			InteractedWithDish(PlayerCharacter);
			break;
		case EItemType::Soap:
					
			break;
		case EItemType::Sanitizer:
					
			break;
		case EItemType::Faucet:
					
			break;
		case EItemType::Plate:
					
			break;
	}
}

bool ARinser::CanInteract()
{
	return bCanInteract;
}

void ARinser::InteractedWithDish(const ADSCharacter* PlayerCharacter)
{
	UE_LOG(LogTemp, Log, TEXT("interact"));
	if (ADish* Dish = Cast<ADish>(PlayerCharacter->HeldItem); Dish->GetDishState() == EDishState::Washed)
		Dish->ProgressDishState();
}
