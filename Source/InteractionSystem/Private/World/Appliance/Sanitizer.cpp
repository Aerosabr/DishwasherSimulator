#include "World/Appliance/Sanitizer.h"

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
	SetWaterMesh();
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
