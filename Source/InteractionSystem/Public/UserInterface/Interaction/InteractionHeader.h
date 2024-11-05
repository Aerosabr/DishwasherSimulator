#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionHeader.generated.h"

struct FInteractableData;
class UTextBlock;
class ADSCharacter;

UCLASS()
class INTERACTIONSYSTEM_API UInteractionHeader : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, Category = "Interaction Header")
	ADSCharacter* PlayerReference;

	void UpdateWidget(const FText& InteractableName) const;

protected:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Header")
	UTextBlock* NameText;
	
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
};
