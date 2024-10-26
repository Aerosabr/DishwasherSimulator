#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Pickup.generated.h"

class ADSCharacter;

UINTERFACE(MinimalAPI)
class UPickup : public UInterface
{
	GENERATED_BODY()
};

class INTERACTIONSYSTEM_API IPickup
{
	GENERATED_BODY()

public:
	virtual void DropItem(ADSCharacter* PlayerCharacter);
};
