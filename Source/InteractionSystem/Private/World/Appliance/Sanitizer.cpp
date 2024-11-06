#include "World/Appliance/Sanitizer.h"
#include "Data/ItemDataStructs.h"
#include "InteractionSystem/DSCharacter.h"
#include "Items/Dish.h"
#include "Items/Disinfectant.h"
#include "Manager/DSManager.h"
#include "UserInterface/InteractionHUD.h"

ASanitizer::ASanitizer()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SanitizerMesh = CreateDefaultSubobject<UStaticMeshComponent>("SanitizerMesh");
	SetRootComponent(SanitizerMesh);

	DisinfectantAmount = 0;
	
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
			InteractedWithDish();
			break;
		case EItemType::Soap:
			
			break;
		case EItemType::Disinfectant:
			InteractedWithDisinfectant();
			break;
	}
	
}

bool ASanitizer::CanInteract()
{
	switch(Player->GetHeldItemType())
	{
		case EItemType::None:
			return false;

		case EItemType::Soap:
			return false;
	
		case EItemType::Disinfectant:
			return Cast<ADisinfectant>(Player->HeldItem)->GetDisinfectantAmount() != 0 && DisinfectantAmount != 160;
		
		case EItemType::Dish:
			return Cast<ADish>(Player->HeldItem)->GetDishState() == EDishState::Rinsed && SanitizerState == ESanitizerState::Sanitized;
		
		default:
			return false;
	}
}

FText ASanitizer::GetInteractionHeader()
{
	FText HeaderText;
	switch (SanitizerState)
	{
		case ESanitizerState::Default:
			HeaderText = FText::FromString("Sanitizer\nClear Water");
			break;
		case ESanitizerState::Sanitized:
			HeaderText = FText::Format(FText::FromString("Sanitizer\nDisinfectant ({0}%)"), FText::AsNumber((DisinfectantAmount * 100) / 160));
			break;
	}
	
	return HeaderText;
}

FText ASanitizer::GetInteractionText()
{
	switch(Player->GetHeldItemType())
	{
		case EItemType::Disinfectant:
			return FText::FromString("Press E To Fill");
			
		case EItemType::Dish:
			return FText::FromString("Press E To Sanitize");
	
		default:
			return FText::FromString("Press E To Sanitize");
	}
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
		DisinfectantAmount = FMath::Max(DisinfectantAmount -= Cast<ADish>(Player->HeldItem)->DebrisAmount, 0);
		if (DisinfectantAmount == 0)
		{
			SanitizerState = ESanitizerState::Default;
			SanitizerMesh->SetMaterial(2, Materials[1]);
		}
		Cast<AInteractionHUD>(GetWorld()->GetFirstPlayerController()->GetHUD())->UpdateInteractionHeader(GetInteractionHeader());
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

void ASanitizer::InteractedWithDish()
{
	if (ADish* TempDish = Cast<ADish>(Player->HeldItem); TempDish->GetDishState() == EDishState::Rinsed)
	{
		Player->ToggleMovement(false);
		Cast<AInteractionHUD>(GetWorld()->GetFirstPlayerController()->GetHUD())->HideInteractionWidget();
		EndFocus();
		StartSanitizing();
	}
}

void ASanitizer::InteractedWithDisinfectant()
{
	ADisinfectant* Disinfectant = Cast<ADisinfectant>(Player->HeldItem);
	
	const int MaxAmount = 160;
	int TransferAmount = MaxAmount - DisinfectantAmount;
	TransferAmount = FMath::Min(Disinfectant->DisinfectantAmount, TransferAmount);

	Disinfectant->DisinfectantAmount -= TransferAmount;
	DisinfectantAmount += TransferAmount;
	
	Cast<AInteractionHUD>(GetWorld()->GetFirstPlayerController()->GetHUD())->HideInteractionWidget();
	EndFocus();
	
	SanitizerState = ESanitizerState::Sanitized;
	SanitizerMesh->SetMaterial(2, Materials[0]);
	Cast<AInteractionHUD>(GetWorld()->GetFirstPlayerController()->GetHUD())->UpdateInteractionHeader(GetInteractionHeader());
}


