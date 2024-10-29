#include "World/Appliance/Sanitizer.h"
#include "Data/ItemDataStructs.h"
#include "InteractionSystem/DSCharacter.h"
#include "Items/Dish.h"


ASanitizer::ASanitizer()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SanitizerMesh = CreateDefaultSubobject<UStaticMeshComponent>("SanitizerMesh");
	SetRootComponent(SanitizerMesh);
}

void ASanitizer::BeginPlay()
{
	Super::BeginPlay();
	
	InteractableData = InstanceInteractableData;
	SanitizerState = ESanitizerState::Sanitized;
	SetWaterMesh();
}

void ASanitizer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASanitizer::BeginFocus()
{
	if (SanitizerMesh)
		SanitizerMesh->SetRenderCustomDepth(true);
}

void ASanitizer::EndFocus()
{
	if (SanitizerMesh)
		SanitizerMesh->SetRenderCustomDepth(false);
}

void ASanitizer::BeginInteract()
{

}

void ASanitizer::EndInteract()
{

}

void ASanitizer::Interact(ADSCharacter* PlayerCharacter)
{
	UE_LOG(LogTemp, Warning, TEXT("Interacting with Sanitizer"));
	switch(PlayerCharacter->GetHeldItemType())
	{
		case EItemType::None:
			SetWaterMesh();
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

bool ASanitizer::CanInteract()
{
	return bCanInteract;
}

void ASanitizer::SetWaterMesh()
{
	switch (SanitizerState)
	{
	case ESanitizerState::Default:
		SanitizerState = ESanitizerState::Sanitized;
		SanitizerMesh->SetMaterial(2, Materials[0]);
		break;
	case ESanitizerState::Sanitized:
		SanitizerState = ESanitizerState::Default;
		SanitizerMesh->SetMaterial(2, Materials[1]);
		break;
	}
}

void ASanitizer::InteractedWithDish(const ADSCharacter* PlayerCharacter)
{
	if (ADish* Dish = Cast<ADish>(PlayerCharacter->HeldItem); Dish->GetDishState() == EDishState::Rinsed)
		Dish->ProgressDishState();
}


