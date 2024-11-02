#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DSManager.generated.h"

class AStartBell;
class ADishSpawner;
class AInteractionHUD;

UCLASS()
class INTERACTIONSYSTEM_API UDSManager : public UGameInstance
{
	GENERATED_BODY()

public:
	void ChangeMoney(int amount);
	void StartDay();
	void AllDishesWashed();
	
	FORCEINLINE bool GetOpen() const { return bOpen; };
	FORCEINLINE int GetMoney() const { return Money; };
	FORCEINLINE int GetDay() const { return Day; };
	FORCEINLINE int GetTime() const { return Time; };
	FORCEINLINE void SetSpawner(ADishSpawner* DishSpawner) { Spawner = DishSpawner; };
	FORCEINLINE void SetBell(AStartBell* StartBell) { Bell = StartBell; };
	
	UPROPERTY(VisibleAnywhere, Category = "DSManager", meta = (DisplayPriority = 0))
	ADishSpawner* Spawner;
	
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

	UPROPERTY(VisibleAnywhere, Category = "DSManager", meta = (DisplayPriority = 0))
	AStartBell* Bell;
	
	virtual void Init() override;
	
	void OnTimerTick();
};
