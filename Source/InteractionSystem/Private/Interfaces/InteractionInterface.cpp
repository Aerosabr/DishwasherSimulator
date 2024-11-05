#include "Interfaces/InteractionInterface.h"

void IInteractionInterface::BeginFocus()
{
}

void IInteractionInterface::EndFocus()
{
}

void IInteractionInterface::Interact(ADSCharacter* PlayerCharacter)
{
}

bool IInteractionInterface::CanInteract()
{
	return false;
}

FText IInteractionInterface::GetInteractionHeader()
{
	return FText::GetEmpty();
}

FText IInteractionInterface::GetInteractionText()
{
	return FText::GetEmpty();
}
