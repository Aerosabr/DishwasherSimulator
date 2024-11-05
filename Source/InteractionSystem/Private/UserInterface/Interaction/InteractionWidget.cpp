#include "UserInterface/Interaction/InteractionWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Interfaces/InteractionInterface.h"

void UInteractionWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UInteractionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void UInteractionWidget::UpdateWidget(const FText& WidgetText) const
{
	InteractionText->SetText(WidgetText);
}



