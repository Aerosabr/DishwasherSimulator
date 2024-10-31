#include "World/Appliance/Sanitizer.h"
#include "Data/ItemDataStructs.h"
#include "InteractionSystem/DSCharacter.h"
#include "Items/Dish.h"
#include "Manager/DSManager.h"

ASanitizer::ASanitizer()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SanitizerMesh = CreateDefaultSubobject<UStaticMeshComponent>("SanitizerMesh");
	SetRootComponent(SanitizerMesh);

	// Scrubbing variables
	bIsScrubbing = false;
	RotationCenter = FVector(0.f, 0.f, 0.f);
	CumulativeDistance = 0.f;
	DistanceThreshold = 0.f;
}

void ASanitizer::BeginPlay()
{
	Super::BeginPlay();

	LastMousePosition = FVector::ZeroVector;
	
	InteractableData = InstanceInteractableData;
	SanitizerState = ESanitizerState::Sanitized;
	SetWaterMesh();
	
	Player = Cast<ADSCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
}

void ASanitizer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsScrubbing)
	{
		FVector MousePosition = GetCurrentMousePosition();
		CalculateDistance(MousePosition);
	}
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

void ASanitizer::Interact(ADSCharacter* PlayerCharacter)
{
	UE_LOG(LogTemp, Warning, TEXT("Interacting with Sanitizer"));
	switch(PlayerCharacter->GetHeldItemType())
	{
		case EItemType::None:
			SetWaterMesh();
			break;
		case EItemType::Dish:
			InteractedWithDish(PlayerCharacter);
			break;
		case EItemType::Soap:
					
			break;
		case EItemType::Sanitizer:
					
			break;
		case EItemType::Faucet:
					
			break;
		case EItemType::Plate:
					
			break;
	}
	
}

bool ASanitizer::CanInteract()
{
	return bCanInteract;
}

void ASanitizer::StartSanitizing()
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
		DistanceThreshold = 4000.0f;
		CumulativeDistance = 0.0f;
	}
}

void ASanitizer::CalculateDistance(FVector MousePosition)
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

FVector ASanitizer::GetCurrentMousePosition()
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

void ASanitizer::InteractedWithDish(ADSCharacter* PlayerCharacter)
{
	if (ADish* TempDish = Cast<ADish>(PlayerCharacter->HeldItem); TempDish->GetDishState() == EDishState::Rinsed)
	{
		PlayerCharacter->ToggleMovement(false);
		StartSanitizing();
	}
}


