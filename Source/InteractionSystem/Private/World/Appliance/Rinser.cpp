#include "World/Appliance/Rinser.h"

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
}

bool ARinser::CanInteract()
{
	return bCanInteract;
}
