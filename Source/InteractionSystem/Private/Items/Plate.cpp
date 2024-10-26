#include "Items/Plate.h"

#include "Data/ItemDataStructs.h"
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
	DishState = EDishState::Dirty;
	Cast<UPrimitiveComponent>(GetComponentByClass(UPrimitiveComponent::StaticClass()))->SetSimulatePhysics(false);
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
			PlayerCharacter->SetIsHoldingItem(true, EItemType::Dish);
			PlayerCharacter->HeldItem = this;
		}
		else
			UE_LOG(LogTemp, Warning, TEXT("HOLDING ITEM"));
	}
}

bool APlate::CanInteract()
{
	return bCanInteract;
}

void APlate::DropItem(ADSCharacter* PlayerCharacter)
{
	// Detach the item from the player
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	// Get the player's viewpoint location and rotation
	FVector ViewLocation;
	FRotator ViewRotation;
	if (PlayerCharacter->GetController())
	{
		PlayerCharacter->GetController()->GetPlayerViewPoint(ViewLocation, ViewRotation);
	}

	// Set the end point for the line trace based on view direction
	FVector EndLocation = ViewLocation + (ViewRotation.Vector() * 500.0f); // Adjust 500 as needed for distance

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); // Ignore the item itself
	QueryParams.AddIgnoredActor(PlayerCharacter); // Ignore the player character

	// Perform the line trace
	if (GetWorld()->LineTraceSingleByChannel(HitResult, ViewLocation, EndLocation, ECC_Visibility, QueryParams))
	{
		// If we hit something, use that location
		const FVector SpawnLocation = HitResult.Location;
		const FTransform SpawnTransform(PlayerCharacter->GetActorRotation(), SpawnLocation);

		SetActorTransform(SpawnTransform);
	}
	else
	{
		// If nothing was hit, fall back to dropping the item at the player’s feet
		const FVector FeetLocation = PlayerCharacter->GetActorLocation() - FVector(0, 0, PlayerCharacter->GetMesh()->GetComponentLocation().Z);
		const FVector SpawnLocation = FeetLocation + (PlayerCharacter->GetActorForwardVector() * 50.0f);
		const FTransform SpawnTransform(PlayerCharacter->GetActorRotation(), SpawnLocation);

		SetActorTransform(SpawnTransform);
	}

	// Enable collision on the item
	Cast<UPrimitiveComponent>(GetComponentByClass(UPrimitiveComponent::StaticClass()))->SetSimulatePhysics(true);
	PlateMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}


