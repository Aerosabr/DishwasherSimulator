#include "World/Appliance/DishRack.h"

#include "Data/ItemDataStructs.h"
#include "InteractionSystem/DSCharacter.h"
#include "Items/Dish.h"
#include "Items/DishSpawner.h"
#include "Manager/DSManager.h"
#include "UserInterface/InteractionHUD.h"

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
	
	Player = Cast<ADSCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
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
			if (UDSManager* GameInstance = Cast<UDSManager>(GetGameInstance()))
				GameInstance->Spawner->SpawnedDishes.RemoveSingle(TempDish);
			Cast<AInteractionHUD>(GetWorld()->GetFirstPlayerController()->GetHUD())->HideInteractionWidget();
			EndFocus();
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
	if (Player->GetHeldItemType() == EItemType::Dish)
		return Cast<ADish>(Player->HeldItem)->GetDishState() == EDishState::Sanitized;

	return false;
}

FText ADishRack::GetInteractionHeader()
{
	return FText::FromString("Dish Rack");
}

FText ADishRack::GetInteractionText()
{
	return FText::FromString("Press E To Submit Dish");
}



