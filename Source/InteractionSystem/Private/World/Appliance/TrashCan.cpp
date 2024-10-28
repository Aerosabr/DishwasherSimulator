#include "World/Appliance/TrashCan.h"

#include "Data/ItemDataStructs.h"
#include "InteractionSystem/DSCharacter.h"
#include "Items/Dish.h"

ATrashCan::ATrashCan()
{
	PrimaryActorTick.bCanEverTick = true;
	
	TrashCanMesh = CreateDefaultSubobject<UStaticMeshComponent>("TrashCanMesh");
	SetRootComponent(TrashCanMesh);
}

void ATrashCan::BeginPlay()
{
	Super::BeginPlay();
	
	InteractableData = InstanceInteractableData;
}

void ATrashCan::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATrashCan::BeginFocus()
{
	if (TrashCanMesh)
		TrashCanMesh->SetRenderCustomDepth(true);
}

void ATrashCan::EndFocus()
{
	if (TrashCanMesh)
		TrashCanMesh->SetRenderCustomDepth(false);
}

void ATrashCan::BeginInteract()
{

}

void ATrashCan::EndInteract()
{

}

void ATrashCan::Interact(ADSCharacter* PlayerCharacter)
{
	UE_LOG(LogTemp, Warning, TEXT("Interacting with TrashCan"));
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

bool ATrashCan::CanInteract()
{
	return bCanInteract;
}

void ATrashCan::InteractedWithDish(const ADSCharacter* PlayerCharacter)
{
	if (ADish* Dish = Cast<ADish>(PlayerCharacter->HeldItem); Dish->GetDishState() == EDishState::Dirty)
		Dish->ProgressDishState();
}
