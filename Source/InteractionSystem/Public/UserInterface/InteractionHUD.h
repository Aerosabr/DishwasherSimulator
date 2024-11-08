#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "InteractionHUD.generated.h"

class UGameWidget;
class UMainMenu;
class UInteractionWidget;
class UInteractionHeader;

UCLASS()
class INTERACTIONSYSTEM_API AInteractionHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> MainMenuClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UInteractionWidget> InteractionWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UInteractionHeader> InteractionHeaderClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> CrosshairClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UGameWidget> GameWidgetClass; 
	
	bool bIsMenuVisible;

	AInteractionHUD();

	void DisplayMenu();
	void HideMenu();
	void ToggleMenu();

	void ShowCrosshair() const;
	void HideCrosshair() const;
	
	void ShowInteractionHeader() const;
	void HideInteractionHeader() const;
	void UpdateInteractionHeader(const FText& InteractableName) const;
	
	void ShowInteractionWidget() const;
	void HideInteractionWidget() const;
	void UpdateInteractionWidget(const FText& InteractionText) const;
	
	void UpdateGameWidgetMoney() const;
	void UpdateGameWidgetTime() const;
	
protected:
	UPROPERTY()
	UMainMenu* MainMenuWidget;

	UPROPERTY(VisibleAnywhere, Category = "Widgets")
	UInteractionWidget* InteractionWidget;

	UPROPERTY(VisibleAnywhere, Category = "Widgets")
	UInteractionHeader* InteractionHeader;
	
	UPROPERTY()
	UUserWidget* CrosshairWidget;

	UPROPERTY()
	UGameWidget* GameWidget;

	virtual void BeginPlay() override;
};
