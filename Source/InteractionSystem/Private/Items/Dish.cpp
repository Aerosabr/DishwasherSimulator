#include "Items/Dish.h"

#include "Data/ItemDataStructs.h"
#include "InteractionSystem/DSCharacter.h"

ADish::ADish()
{
	PrimaryActorTick.bCanEverTick = true;

	DishMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
	DishMesh->SetSimulatePhysics(true);
	SetRootComponent(DishMesh);
}

void ADish::BeginPlay()
{
	Super::BeginPlay();
	InteractableData = InstanceInteractableData;
	DishState = EDishState::Dirty;
	Cast<UPrimitiveComponent>(GetComponentByClass(UPrimitiveComponent::StaticClass()))->SetSimulatePhysics(false);
}

void ADish::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADish::BeginFocus()
{
	if (DishMesh)
		DishMesh->SetRenderCustomDepth(true);
}

void ADish::EndFocus()
{
	if (DishMesh)
		DishMesh->SetRenderCustomDepth(false);
}

void ADish::BeginInteract()
{
	IInteractionInterface::BeginInteract();
}

void ADish::EndInteract()
{
	IInteractionInterface::EndInteract();
}

void ADish::Interact(ADSCharacter* PlayerCharacter)
{
	if (PlayerCharacter)
	{
		if (!PlayerCharacter->GetIsHoldingItem())
		{
			UE_LOG(LogTemp, Warning, TEXT("NOT HOLDING ITEM"));
			Cast<UPrimitiveComponent>(GetComponentByClass(UPrimitiveComponent::StaticClass()))->SetSimulatePhysics(false);
			DishMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			AttachToComponent(PlayerCharacter->GetMesh1P(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("AttachSocket"));
			PlayerCharacter->SetIsHoldingItem(true, EItemType::Dish);
			PlayerCharacter->HeldItem = this;
		}
		else
			UE_LOG(LogTemp, Warning, TEXT("HOLDING ITEM"));
	}
}

bool ADish::CanInteract()
{
	return bCanInteract;
}

void ADish::DropItem(ADSCharacter* PlayerCharacter)
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
	DishMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ADish::ProgressDishState()
{
	switch (DishState)
	{
		case EDishState::Dirty:
			DishState = EDishState::Scraped;
			SetDishMesh();
			break;
		case EDishState::Scraped:
			
			break;
		case EDishState::Washed:
			
			break;
		case EDishState::Rinsed:
			
			break;
		case EDishState::Sanitized:
			
			break;
	}
}

void ADish::SetDishMesh()
{
	switch (DishState)
	{
	case EDishState::Dirty:
		DishMesh->SetStaticMesh(DishMeshes[0]);
		break;
	case EDishState::Scraped:
		DishMesh->SetStaticMesh(DishMeshes[1]);
		break;
	case EDishState::Washed:
		DishMesh->SetStaticMesh(DishMeshes[2]);
		break;
	case EDishState::Rinsed:
		DishMesh->SetStaticMesh(DishMeshes[3]);
		break;
	case EDishState::Sanitized:
		DishMesh->SetStaticMesh(DishMeshes[4]);
		break;
	default:
		DishMesh->SetStaticMesh(DishMeshes[0]);
		break;
	}
}

void ADish::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName ChangedPropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(FDataTableRowHandle, RowName))
	{
		if (!ItemRowHandle.IsNull())
		{
			UE_LOG(LogTemp, Log, TEXT("PostEdit"));
			const FItemData* ItemData = ItemRowHandle.GetRow<FItemData>(ItemRowHandle.RowName.ToString());
			DishMeshes = ItemData->AssetData.Mesh;
			SetDishMesh();
		}
	}
}


