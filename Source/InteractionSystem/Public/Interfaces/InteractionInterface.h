#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

class ADSCharacter;

UINTERFACE(MinimalAPI)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

class INTERACTIONSYSTEM_API IInteractionInterface
{
	GENERATED_BODY()

public:
	
	virtual void BeginFocus();
	virtual void EndFocus();
	virtual void Interact(ADSCharacter* PlayerCharacter);

	virtual bool CanInteract();
	virtual FText GetInteractionHeader();
	virtual FText GetInteractionText();
};
