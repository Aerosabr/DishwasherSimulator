#include "World/Appliance/Washer.h"
#include "Data/ItemDataStructs.h"
#include "InteractionSystem/DSCharacter.h"
#include "Items/Dish.h"
#include "Items/Soap.h"

AWasher::AWasher()
{
	PrimaryActorTick.bCanEverTick = true;
	
	WasherMesh = CreateDefaultSubobject<UStaticMeshComponent>("WasherMesh");
	SetRootComponent(WasherMesh);

	SoapAmount = 0;
	
	// Scrubbing variables
	bIsScrubbing = false;
	RotationCenter = FVector(0.f, 0.f, 0.f);
	CumulativeDistance = 0.f;
	DistanceThreshold = 0.f;
}

void AWasher::BeginPlay()
{
	Super::BeginPlay();

	LastMousePosition = FVector::ZeroVector;
	
	WasherState = EWasherState::Dirty;
	SetWaterMesh();
	
	Player = Cast<ADSCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
}

void AWasher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsScrubbing)
	{
		FVector MousePosition = GetCurrentMousePosition();
		CalculateDistance(MousePosition);
	}
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

void AWasher::Interact(ADSCharacter* PlayerCharacter)
{
	UE_LOG(LogTemp, Warning, TEXT("Interacting with Washer"));
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

bool AWasher::CanInteract()
{
	switch(Player->GetHeldItemType())
	{
		case EItemType::None:
			return WasherState == EWasherState::Dirty;

		case EItemType::Soap:
			return Cast<ASoap>(Player->HeldItem)->GetSoapAmount() != 0;
		
		case EItemType::Disinfectant:
			return false;
			
		case EItemType::Dish:
			if (Cast<ADish>(Player->HeldItem)->GetDishState() == EDishState::Scraped)
				return true;
			return false;
		default:
			return false;
	}
}

FText AWasher::GetInteractionHeader()
{
	return FText::FromString("Washer");
}

FText AWasher::GetInteractionText()
{
	return FText::FromString("Press E To Wash");
}

void AWasher::StartWashing()
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
		DistanceThreshold = 6000.0f;
		CumulativeDistance = 0.0f;
	}
}

void AWasher::CalculateDistance(FVector MousePosition)
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

FVector AWasher::GetCurrentMousePosition()
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

void AWasher::InteractedWithDish(ADSCharacter* PlayerCharacter)
{
	if (ADish* TempDish = Cast<ADish>(PlayerCharacter->HeldItem); TempDish->GetDishState() == EDishState::Scraped)
	{
		PlayerCharacter->ToggleMovement(false);
		StartWashing();
	}
}
