#include "World/Appliance/TrashCan.h"
#include "Data/ItemDataStructs.h"
#include "InteractionSystem/DSCharacter.h"
#include "Items/Dish.h"
#include "Items/Disinfectant.h"
#include "Items/Soap.h"
#include "Manager/DSManager.h"
#include "UserInterface/InteractionHUD.h"

ATrashCan::ATrashCan()
{
	PrimaryActorTick.bCanEverTick = true;
	
	TrashCanMesh = CreateDefaultSubobject<UStaticMeshComponent>("TrashCanMesh");
	SetRootComponent(TrashCanMesh);

	// Scrubbing variables
	bIsScrubbing = false;
	RotationCenter = FVector(0.f, 0.f, 0.f);
	CumulativeDistance = 0.f;
	DistanceThreshold = 0.f;
}

void ATrashCan::BeginPlay()
{
	Super::BeginPlay();

	LastMousePosition = FVector::ZeroVector;

	Player = Cast<ADSCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	HUD = Cast<AInteractionHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
}

void ATrashCan::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsScrubbing)
	{
		FVector MousePosition = GetCurrentMousePosition();
		CalculateDistance(MousePosition);
	}
}

void ATrashCan::BeginFocus()
{
	if (TrashCanMesh)
		TrashCanMesh->SetRenderCustomDepth(true);
}

void ATrashCan::EndFocus()
{
	if (TrashCanMesh)
		TrashCanMesh->SetRenderCustomDepth(false);
}

void ATrashCan::Interact(ADSCharacter* PlayerCharacter)
{
	UE_LOG(LogTemp, Warning, TEXT("Interacting with TrashCan"));
	switch(PlayerCharacter->GetHeldItemType())
	{
		case EItemType::None:
			
			break;
		case EItemType::Dish:
			InteractedWithDish(PlayerCharacter);
			break;
		case EItemType::Soap:
			HUD->HideInteractionWidget();
			EndFocus();
			Cast<UDSManager>(GetGameInstance())->SoapBottles.Remove(Cast<ASoap>(PlayerCharacter->HeldItem));
			PlayerCharacter->HeldItem->Destroy();
			PlayerCharacter->HeldItem = nullptr;
			PlayerCharacter->SetIsHoldingItem(false, EItemType::None);
			break;
		case EItemType::Disinfectant:
			HUD->HideInteractionWidget();
			EndFocus();
			Cast<UDSManager>(GetGameInstance())->DisinfectantBottles.Remove(Cast<ADisinfectant>(PlayerCharacter->HeldItem));
			PlayerCharacter->HeldItem->Destroy();
			PlayerCharacter->HeldItem = nullptr;
			PlayerCharacter->SetIsHoldingItem(false, EItemType::None);
			break;
	}
}

bool ATrashCan::CanInteract()
{
	switch(Player->GetHeldItemType())
	{
		case EItemType::None:
			return false;

		case EItemType::Soap:
			return true;
	
		case EItemType::Disinfectant:
			return true;

		case EItemType::Dish:
			return Cast<ADish>(Player->HeldItem)->GetDishState() == EDishState::Dirty;
	}
	
	return false;
}

FText ATrashCan::GetInteractionHeader()
{
	return FText::FromString("Trash Can");
}

FText ATrashCan::GetInteractionText()
{
	if (bIsScrubbing)
		return FText::Format(FText::FromString("Scrape Progress: {0}%"), FText::AsNumber(FMath::RoundToFloat(((CumulativeDistance * 100) / DistanceThreshold) * 100.0f) / 100.0f));
	
	switch(Player->GetHeldItemType())
	{
		case EItemType::Soap:
			return FText::FromString("Press E To Throw Away");
		
		case EItemType::Disinfectant:
			return FText::FromString("Press E To Throw Away");

		case EItemType::Dish:
			return FText::FromString("Press E To Scrape Waste");
		default:
			return FText::FromString("Press E To Throw Away");
	}
}

void ATrashCan::StartScraping()
{
	if (!bIsScrubbing)
	{
		//HUD->HideInteractionWidget();
		HUD->HideCrosshair();
		EndFocus();
		Player->HeldItem->AttachToComponent(Player->GetMesh1P(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("Interacting"));
		
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
		DistanceThreshold = 150 * Cast<ADish>(Player->HeldItem)->DebrisAmount;
		CumulativeDistance = 0.0f;
	}
}

void ATrashCan::CalculateDistance(FVector MousePosition)
{
	if (LastMousePosition.IsZero())
	{
		LastMousePosition = MousePosition;
		return;
	}

	HUD->UpdateInteractionWidget(GetInteractionText());
	
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
		HUD->ShowCrosshair();
		Player->HeldItem->AttachToComponent(Player->GetMesh1P(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("AttachSocket"));
		Cast<ADish>(Player->HeldItem)->ProgressDishState();
		Player->ToggleMovement(true);
		FSlateApplication::Get().ClearKeyboardFocus(EFocusCause::Cleared);
	}

	LastMousePosition = MousePosition;
}

FVector ATrashCan::GetCurrentMousePosition()
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

void ATrashCan::InteractedWithDish(ADSCharacter* PlayerCharacter)
{
	if (ADish* TempDish = Cast<ADish>(PlayerCharacter->HeldItem); TempDish->GetDishState() == EDishState::Dirty)
	{
		PlayerCharacter->ToggleMovement(false);
		StartScraping();
	}
}
