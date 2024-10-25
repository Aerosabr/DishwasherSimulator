#include "World/Appliance/TrashCan.h"

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
}

bool ATrashCan::CanInteract()
{
	return bCanInteract;
}
