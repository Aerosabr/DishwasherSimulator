#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameWidget.generated.h"
class ADSCharacter;
class UProgressBar;
class UTextBlock;

UCLASS()
class INTERACTIONSYSTEM_API UGameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Game Widget")
	UTextBlock* Day;
	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Game Widget")
	UTextBlock* Money;
	
	void UpdateWidget();
	void ChangeMoney();
	
protected:
	
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
};
