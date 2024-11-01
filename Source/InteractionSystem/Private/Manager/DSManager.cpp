#include "Manager/DSManager.h"
#include "Engine/Engine.h"
#include "TimerManager.h"
#include "UserInterface/InteractionHUD.h"

void UDSManager::Init()
{
	Super::Init();

	Day = 1;
	Money = 0;
	Time = 36000;
	bOpen = false;
}

void UDSManager::OnTimerTick()
{
	Time += 300;
	Cast<AInteractionHUD>(GetWorld()->GetFirstPlayerController()->GetHUD())->UpdateGameWidgetTime();
}

void UDSManager::ChangeMoney(int amount)
{
	Money += amount;
	Cast<AInteractionHUD>(GetWorld()->GetFirstPlayerController()->GetHUD())->UpdateGameWidgetMoney();
}

void UDSManager::ToggleTimer()
{
	if (!bOpen)
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle,this, &UDSManager::OnTimerTick, 1.0f, true);
		bOpen = true;
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		bOpen = false;
	}
}




