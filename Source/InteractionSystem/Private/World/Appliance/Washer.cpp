#include "World/Appliance/Washer.h"
#include "Data/ItemDataStructs.h"
#include "InteractionSystem/DSCharacter.h"
#include "Items/Dish.h"
#include "Items/Soap.h"
#include "Manager/DSManager.h"
#include "UserInterface/InteractionHUD.h"

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
	
	WasherState = EWasherState::Default;
	SetWaterMesh();
	
	Player = Cast<ADSCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	HUD = Cast<AInteractionHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	
	Cast<UDSManager>(GetGameInstance())->Washer = this;
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
			WasherState = EWasherState::Default;
			WasherMesh->SetMaterial(0, Materials[2]);
			HUD->UpdateInteractionHeader(GetInteractionHeader());
			HUD->HideInteractionWidget();
			EndFocus();
			break;
		case EItemType::Dish:
			InteractedWithDish();
			break;
		case EItemType::Soap:
			InteractedWithSoap();
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
			return Cast<ASoap>(Player->HeldItem)->GetSoapAmount() != 0 && SoapAmount != 160 && WasherState != EWasherState::Dirty;
		
		case EItemType::Disinfectant:
			return false;
			
		case EItemType::Dish:
			return Cast<ADish>(Player->HeldItem)->GetDishState() == EDishState::Scraped && WasherState == EWasherState::Soap;
		default:
			return false;
	}
}

FText AWasher::GetInteractionHeader()
{
	FText HeaderText;
	switch (WasherState)
	{
		case EWasherState::Default:
			HeaderText = FText::FromString("Washer\nClear Water");
			break;
		case EWasherState::Soap:
			HeaderText = FText::Format(FText::FromString("Washer\nSoapy Water ({0}%)"), FText::AsNumber((SoapAmount * 100) / 160));
			break;
		case EWasherState::Dirty:
			HeaderText = FText::FromString("Washer\nDirty Water");
			break;
	}
	
	return HeaderText;
}

FText AWasher::GetInteractionText()
{
	if (bIsScrubbing)
		return FText::Format(FText::FromString("Wash Progress: {0}%"), FText::AsNumber(FMath::RoundToFloat(((CumulativeDistance * 100) / DistanceThreshold) * 100.0f) / 100.0f));
	
	switch(Player->GetHeldItemType())
	{
		case EItemType::None:
			return FText::FromString("Press E To Clear Water");

		case EItemType::Soap:
			return FText::FromString("Press E To Fill");
	
		case EItemType::Dish:
			return FText::FromString("Press E To Wash");
		
		default:
			return FText::FromString("Press E To Wash");
	}
}

void AWasher::GetWasherState(EWasherState& State, int& Amount)
{
	State = WasherState; 
	Amount = SoapAmount;
}

void AWasher::SetWasherState(EWasherState State, int Amount)
{
	WasherState = State;
	SoapAmount = Amount;
	SetWaterMesh();
}

void AWasher::StartWashing()
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
		DistanceThreshold = 300 * Cast<ADish>(Player->HeldItem)->DebrisAmount;
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

		SoapAmount = FMath::Max(SoapAmount -= Cast<ADish>(Player->HeldItem)->DebrisAmount, 0);
		if (SoapAmount == 0)
		{
			WasherState = EWasherState::Dirty;
			WasherMesh->SetMaterial(0, Materials[1]);
		}
		
		HUD->UpdateInteractionHeader(GetInteractionHeader());
		HUD->ShowCrosshair();
		Player->HeldItem->AttachToComponent(Player->GetMesh1P(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("AttachSocket"));
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
	case EWasherState::Soap:
		WasherMesh->SetMaterial(0, Materials[0]);
		break;
	case EWasherState::Dirty:
		WasherMesh->SetMaterial(0, Materials[1]);
		break;
	case EWasherState::Default:
		WasherMesh->SetMaterial(0, Materials[2]);
		break;
	}
}

void AWasher::InteractedWithDish()
{
	if (ADish* TempDish = Cast<ADish>(Player->HeldItem); TempDish->GetDishState() == EDishState::Scraped)
	{
		Player->ToggleMovement(false);
		StartWashing();
	}
}

void AWasher::InteractedWithSoap()
{
	ASoap* Soap = Cast<ASoap>(Player->HeldItem);
	
	const int MaxAmount = 160;
	int TransferAmount = MaxAmount - SoapAmount;
	TransferAmount = FMath::Min(Soap->SoapAmount, TransferAmount);

	Soap->SoapAmount -= TransferAmount;
	SoapAmount += TransferAmount;
	
	HUD->HideInteractionWidget();
	EndFocus();
	
	WasherState = EWasherState::Soap;
	WasherMesh->SetMaterial(0, Materials[0]);
	HUD->UpdateInteractionHeader(GetInteractionHeader());
}