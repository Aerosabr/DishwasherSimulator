#include "Items/Plate.h"

#include "InteractionSystem/DSCharacter.h"

APlate::APlate()
{
	PrimaryActorTick.bCanEverTick = true;

	PlateMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
	PlateMesh->SetSimulatePhysics(true);
	SetRootComponent(PlateMesh);
}

void APlate::BeginPlay()
{
	Super::BeginPlay();
	InteractableData = InstanceInteractableData;
}

void APlate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlate::BeginFocus()
{
	if (PlateMesh)
		PlateMesh->SetRenderCustomDepth(true);
}

void APlate::EndFocus()
{
	if (PlateMesh)
		PlateMesh->SetRenderCustomDepth(false);
}

void APlate::BeginInteract()
{
	IInteractionInterface::BeginInteract();
}

void APlate::EndInteract()
{
	IInteractionInterface::EndInteract();
}

void APlate::Interact(ADSCharacter* PlayerCharacter)
{
	if (PlayerCharacter)
	{
		if (!PlayerCharacter->GetIsHoldingItem())
		{
			UE_LOG(LogTemp, Warning, TEXT("NOT HOLDING ITEM"));
			Cast<UPrimitiveComponent>(GetComponentByClass(UPrimitiveComponent::StaticClass()))->SetSimulatePhysics(false);
			PlateMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			AttachToComponent(PlayerCharacter->GetMesh1P(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("AttachSocket"));
			PlayerCharacter->SetIsHoldingItem(true);
		}
		else
			UE_LOG(LogTemp, Warning, TEXT("HOLDING ITEM"));
	}
}

bool APlate::CanInteract()
{
	return bCanInteract;
}

