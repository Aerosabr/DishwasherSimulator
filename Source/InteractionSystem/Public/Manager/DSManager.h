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
	void ToggleTimer();

	FORCEINLINE int GetMoney() const { return Money; };
	FORCEINLINE int GetDay() const { return Day; };
	FORCEINLINE int GetTime() const { return Time; };
	
protected:
	UPROPERTY(EditAnywhere, Category = "DSManager", meta = (DisplayPriority = 0))
	int Money;

	UPROPERTY(EditAnywhere, Category = "DSManager", meta = (DisplayPriority = 0))
	int Day;

	UPROPERTY(EditAnywhere, Category = "DSManager", meta = (DisplayPriority = 0))
	int Time;

	UPROPERTY(EditAnywhere, Category = "DSManager", meta = (DisplayPriority = 0))
	bool bOpen;
	FTimerHandle TimerHandle;
	
	virtual void Init() override;
	
	void OnTimerTick();
};
