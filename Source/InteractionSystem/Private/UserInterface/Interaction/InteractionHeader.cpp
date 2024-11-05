#include "UserInterface/Interaction/InteractionHeader.h"
#include "Components/TextBlock.h"
#include "Interfaces/InteractionInterface.h"

void UInteractionHeader::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UInteractionHeader::NativeConstruct()
{
	Super::NativeConstruct();
}

void UInteractionHeader::UpdateWidget(const FText& InteractableName) const
{
	NameText->SetText(InteractableName);
}
