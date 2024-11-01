// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "InteractionHUD.generated.h"

class UGameWidget;
struct FInteractableData;
class UMainMenu;
class UInteractionWidget;

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
	TSubclassOf<UUserWidget> CrosshairClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UGameWidget> GameWidgetClass; 
	
	bool bIsMenuVisible;

	AInteractionHUD();

	void DisplayMenu();
	void HideMenu();
	void ToggleMenu();
	
	void ShowInteractionWidget() const;
	void HideInteractionWidget() const;
	void UpdateInteractionWidget(const FInteractableData* InteractableData) const;

	void UpdateGameWidgetMoney() const;
	void UpdateGameWidgetDay() const;
	void UpdateGameWidgetTime() const;
protected:
	UPROPERTY()
	UMainMenu* MainMenuWidget;

	UPROPERTY(VisibleAnywhere, Category = "Interaction Widget")
	UInteractionWidget* InteractionWidget;

	UPROPERTY()
	UUserWidget* CrosshairWidget;

	UPROPERTY()
	UGameWidget* GameWidget;

	virtual void BeginPlay() override;
};
