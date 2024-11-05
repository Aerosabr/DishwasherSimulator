#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionWidget.generated.h"

class ADSCharacter;
class UTextBlock;
class UProgressBar;

UCLASS()
class INTERACTIONSYSTEM_API UInteractionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = "Interaction Widget | Player Reference")
	ADSCharacter* PlayerReference;

	void UpdateWidget(const FText& WidgetText) const;

protected:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
	UTextBlock* InteractionText;
	
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
};
