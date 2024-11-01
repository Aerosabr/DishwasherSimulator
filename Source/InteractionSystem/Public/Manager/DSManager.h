#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DSManager.generated.h"

class AInteractionHUD;

UCLASS()
class INTERACTIONSYSTEM_API UDSManager : public UGameInstance
{
	GENERATED_BODY()

public:
	void ChangeMoney(int amount);
	
protected:
	UPROPERTY(EditAnywhere, Category = "DSManager", meta = (DisplayPriority = 0))
	int Money;

	UPROPERTY(EditAnywhere, Category = "DSManager", meta = (DisplayPriority = 0))
	int Day;

	UPROPERTY(EditAnywhere, Category = "DSManager", meta = (DisplayPriority = 0))
	int Time;
	
	virtual void Init() override;
};
