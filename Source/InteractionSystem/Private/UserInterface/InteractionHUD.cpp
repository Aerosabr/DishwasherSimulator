#include "UserInterface/InteractionHUD.h"

#include "Components/TextBlock.h"
#include "Manager/DSManager.h"
#include "UserInterface/MainMenu.h"
#include "UserInterface/Interaction/InteractionWidget.h"
#include "UserInterface/GameWidget.h"
AInteractionHUD::AInteractionHUD()
{
}

void AInteractionHUD::BeginPlay()
{
	Super::BeginPlay();
	
	if (MainMenuClass)
	{
		MainMenuWidget = CreateWidget<UMainMenu>(GetWorld(), MainMenuClass);
		MainMenuWidget->AddToViewport(5);
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (InteractionWidgetClass)
	{
		InteractionWidget = CreateWidget<UInteractionWidget>(GetWorld(), InteractionWidgetClass);
		InteractionWidget->AddToViewport(-1);
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (GameWidgetClass)
	{
		GameWidget = CreateWidget<UGameWidget>(GetWorld(), GameWidgetClass);
		GameWidget->AddToViewport(5);
		GameWidget->SetVisibility(ESlateVisibility::Visible);
	}
	
	CrosshairWidget = CreateWidget<UUserWidget>(GetWorld(), CrosshairClass);
	CrosshairWidget->AddToViewport(5);
	CrosshairWidget->SetVisibility(ESlateVisibility::Visible);
}

void AInteractionHUD::DisplayMenu()
{
	if (MainMenuWidget)
	{
		bIsMenuVisible = true;
		MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AInteractionHUD::HideMenu()
{
	if (MainMenuWidget)
	{
		bIsMenuVisible = false;
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AInteractionHUD::ToggleMenu()
{
	if (bIsMenuVisible)
	{
		HideMenu();

		const FInputModeGameOnly InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(false);
	}
	else
	{
		DisplayMenu();
		
		const FInputModeGameAndUI InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(true);
	}
}

void AInteractionHUD::ShowInteractionWidget() const
{
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AInteractionHUD::HideInteractionWidget() const
{
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AInteractionHUD::UpdateInteractionWidget(const FInteractableData* InteractableData) const
{
	if (InteractionWidget)
	{
		if (InteractionWidget->GetVisibility() == ESlateVisibility::Collapsed)
			InteractionWidget->SetVisibility(ESlateVisibility::Visible);

		InteractionWidget->UpdateWidget(InteractableData);
	}
}

void AInteractionHUD::UpdateGameWidgetMoney() const
{
	if (UDSManager* GameInstance = Cast<UDSManager>(GetGameInstance()))
		GameWidget->Money->SetText(FText::Format(FText::FromString(TEXT("${0}")), FText::AsNumber(GameInstance->GetMoney())));
	
}

void AInteractionHUD::UpdateGameWidgetDay() const
{
	if (UDSManager* GameInstance = Cast<UDSManager>(GetGameInstance()))
		GameWidget->Day->SetText(FText::AsNumber(GameInstance->GetDay()));
	
}

void AInteractionHUD::UpdateGameWidgetTime() const
{
	if (const UDSManager* GameInstance = Cast<UDSManager>(GetGameInstance()))
	{
		const int TotalSeconds = GameInstance->GetTime();
		int Hours = (TotalSeconds / 3600) % 24;  
		const int Minutes = (TotalSeconds % 3600) / 60;

		const FString Period = (Hours >= 12) ? TEXT("PM") : TEXT("AM");
		Hours = Hours % 12;
		if (Hours == 0) Hours = 12;  
		
		GameWidget->Time->SetText(FText::FromString(FString::Printf(TEXT("%02d:%02d %s"), Hours, Minutes, *Period)));
	}
}

