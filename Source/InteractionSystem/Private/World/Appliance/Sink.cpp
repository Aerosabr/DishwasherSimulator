#include "World/Appliance/Sink.h"

ASink::ASink()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SinkMesh = CreateDefaultSubobject<UStaticMeshComponent>("SinkMesh");
	SetRootComponent(SinkMesh);
	
	WaterMesh = CreateDefaultSubobject<UStaticMeshComponent>("WaterMesh");
}

void ASink::BeginPlay()
{
	Super::BeginPlay();
	
	InteractableData = InstanceInteractableData;
	WaterState = EWaterState::Dirty;
	SetWaterMesh();
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

}

void ASink::EndInteract()
{

}

void ASink::Interact(ADSCharacter* PlayerCharacter)
{
	UE_LOG(LogTemp, Warning, TEXT("Interacting with Sink"));
	SetWaterMesh();
}

bool ASink::CanInteract()
{
	return bCanInteract;
}

void ASink::SetWaterMesh()
{
	switch (WaterState)
	{
		case EWaterState::Default:
			WaterState = EWaterState::Soap;
			SinkMesh->SetMaterial(0, Materials[0]);
			break;
		case EWaterState::Soap:
			WaterState = EWaterState::Dirty;
			SinkMesh->SetMaterial(0, Materials[1]);
			break;
		case EWaterState::Dirty:
			WaterState = EWaterState::Default;
			SinkMesh->SetMaterial(0, Materials[2]);
			break;
		default:
			WaterState = EWaterState::Default;
			SinkMesh->SetMaterial(0, Materials[2]);
			break;
	}
}


