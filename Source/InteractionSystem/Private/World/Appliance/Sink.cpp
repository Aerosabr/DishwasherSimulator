#include "World/Appliance/Sink.h"
#include "Items/ItemBase.h"

ASink::ASink()
{
	PrimaryActorTick.bCanEverTick = true;

	WaterMesh = CreateDefaultSubobject<UStaticMeshComponent>("WaterMesh");
	SetRootComponent(WaterMesh);
	
	SinkMesh = CreateDefaultSubobject<UStaticMeshComponent>("SinkMesh");
	SetRootComponent(SinkMesh);
}

void ASink::BeginPlay()
{
	Super::BeginPlay();

	InteractableData = InstanceInteractableData;
}

void ASink::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASink::BeginFocus()
{
	if (SinkMesh)
		SinkMesh->SetRenderCustomDepth(true);
}

void ASink::EndFocus()
{
	if (SinkMesh)
		SinkMesh->SetRenderCustomDepth(false);
}

void ASink::BeginInteract()
{
	UE_LOG(LogTemp, Warning, TEXT("Calling BeginInteract override on interface test actor."));
}

void ASink::EndInteract()
{
	UE_LOG(LogTemp, Warning, TEXT("Calling EndInteract override on interface test actor."));
}

void ASink::Interact(ADSCharacter* PlayerCharacter)
{
	UE_LOG(LogTemp, Warning, TEXT("Calling nteract override on interface test actor."));
}

bool ASink::CanInteract()
{
	return bCanInteract;
}


