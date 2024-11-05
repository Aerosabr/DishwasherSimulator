#include "World/Appliance/Rinser.h"
#include "Data/ItemDataStructs.h"
#include "InteractionSystem/DSCharacter.h"
#include "Items/Dish.h"

ARinser::ARinser()
{
	PrimaryActorTick.bCanEverTick = true;
	
	RinserMesh = CreateDefaultSubobject<UStaticMeshComponent>("RinserMesh");
	SetRootComponent(RinserMesh);

	// Scrubbing variables
	bIsScrubbing = false;
	RotationCenter = FVector(0.f, 0.f, 0.f);
	CumulativeDistance = 0.f;
	DistanceThreshold = 0.f;
}

void ARinser::BeginPlay()
{
	Super::BeginPlay();

	LastMousePosition = FVector::ZeroVector;

	Player = Cast<ADSCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
}

void ARinser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsScrubbing)
	{
		FVector MousePosition = GetCurrentMousePosition();
		CalculateDistance(MousePosition);
	}
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

void ARinser::Interact(ADSCharacter* PlayerCharacter)
{
	UE_LOG(LogTemp, Warning, TEXT("Interacting with Rinser"));
	switch(PlayerCharacter->GetHeldItemType())
	{
		case EItemType::None:
			
			break;
		case EItemType::Dish:
			InteractedWithDish(PlayerCharacter);
			break;
		case EItemType::Soap:
					
			break;
		case EItemType::Disinfectant:
					
			break;
	}
}

bool ARinser::CanInteract()
{
	if (Player->GetHeldItemType() == EItemType::Dish)
		return Cast<ADish>(Player->HeldItem)->GetDishState() == EDishState::Washed;

	return false;
}

FText ARinser::GetInteractionHeader()
{
	return FText::FromString("Rinser");
}

FText ARinser::GetInteractionText()
{
	return FText::FromString("Press E To Rinse");
}

void ARinser::StartRinsing()
{
	if (!bIsScrubbing)
	{
		if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
		{

			FInputModeUIOnly InputMode;
			
			PlayerController->SetInputMode(InputMode);
			PlayerController->bShowMouseCursor = true;

			FVector2D ViewportSize;
			GetWorld()->GetGameViewport()->GetViewportSize(ViewportSize);
			PlayerController->SetMouseLocation(ViewportSize.X / 2, ViewportSize.Y / 2);
		}
		bIsScrubbing = true;
		DistanceThreshold = 5000.0f;
		CumulativeDistance = 0.0f;
	}
}

void ARinser::CalculateDistance(FVector MousePosition)
{
	if (LastMousePosition.IsZero())
	{
		LastMousePosition = MousePosition;
		return;
	}

	float DistanceMoved = FVector::Dist(MousePosition, LastMousePosition);
	CumulativeDistance += DistanceMoved;
	if (CumulativeDistance >= DistanceThreshold)
	{
		UE_LOG(LogTemp, Log, TEXT("Distance Threshold Reached! Total Distance: %f"), CumulativeDistance);
		CumulativeDistance = 0.f;
		
		bIsScrubbing = false;
		if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
		{
			PlayerController->SetInputMode(FInputModeGameOnly());
			PlayerController->bShowMouseCursor = false;
		}
		
		Cast<ADish>(Player->HeldItem)->ProgressDishState();
		Player->ToggleMovement(true);
		FSlateApplication::Get().ClearKeyboardFocus(EFocusCause::Cleared);
	}

	LastMousePosition = MousePosition;
}

FVector ARinser::GetCurrentMousePosition()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		float ScreenX, ScreenY;
		PlayerController->GetMousePosition(ScreenX, ScreenY);

		FVector WorldLocation, WorldDirection;
		if (PlayerController->DeprojectScreenPositionToWorld(ScreenX, ScreenY, WorldLocation, WorldDirection))
		{
			FVector MousePositionInWorld = WorldLocation + WorldDirection * 1000.f;
			return MousePositionInWorld;
		}
	}

	return FVector::ZeroVector;
}


void ARinser::InteractedWithDish(ADSCharacter* PlayerCharacter)
{
	if (ADish* TempDish = Cast<ADish>(PlayerCharacter->HeldItem); TempDish->GetDishState() == EDishState::Washed)
	{
		PlayerCharacter->ToggleMovement(false);
		StartRinsing();
	}
}
