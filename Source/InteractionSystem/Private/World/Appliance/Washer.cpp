#include "World/Appliance/Washer.h"

AWasher::AWasher()
{
	PrimaryActorTick.bCanEverTick = true;
	
	WasherMesh = CreateDefaultSubobject<UStaticMeshComponent>("WasherMesh");
	SetRootComponent(WasherMesh);
}

void AWasher::BeginPlay()
{
	Super::BeginPlay();
	
	InteractableData = InstanceInteractableData;
	WasherState = EWasherState::Dirty;
	SetWaterMesh();
}

void AWasher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWasher::BeginFocus()
{
	if (WasherMesh)
		WasherMesh->SetRenderCustomDepth(true);
}

void AWasher::EndFocus()
{
	if (WasherMesh)
		WasherMesh->SetRenderCustomDepth(false);
}

void AWasher::BeginInteract()
{

}

void AWasher::EndInteract()
{

}

void AWasher::Interact(ADSCharacter* PlayerCharacter)
{
	UE_LOG(LogTemp, Warning, TEXT("Interacting with Washer"));
	SetWaterMesh();
}

bool AWasher::CanInteract()
{
	return bCanInteract;
}

void AWasher::SetWaterMesh()
{
	switch (WasherState)
	{
	case EWasherState::Default:
		WasherState = EWasherState::Soap;
		WasherMesh->SetMaterial(0, Materials[0]);
		break;
	case EWasherState::Soap:
		WasherState = EWasherState::Dirty;
		WasherMesh->SetMaterial(0, Materials[1]);
		break;
	case EWasherState::Dirty:
		WasherState = EWasherState::Default;
		WasherMesh->SetMaterial(0, Materials[2]);
		break;
	}
}