#include "Manager/DSManager.h"
#include "Engine/Engine.h"
#include "TimerManager.h"
#include "Items/DishSpawner.h"
#include "UserInterface/InteractionHUD.h"
#include "World/Appliance/StartBell.h"

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
	Time += 1200;
	
	if (Time >= 79200)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		bOpen = false;
		Spawner->StopSpawning();
		Bell->bCanInteract = true;
	}
	
	Cast<AInteractionHUD>(GetWorld()->GetFirstPlayerController()->GetHUD())->UpdateGameWidgetTime();
}

void UDSManager::ChangeMoney(int amount)
{
	Money += amount;
	Cast<AInteractionHUD>(GetWorld()->GetFirstPlayerController()->GetHUD())->UpdateGameWidgetMoney();
}

void UDSManager::StartDay()
{
	if (!bOpen)
	{
		Time = 36000;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle,this, &UDSManager::OnTimerTick, 1.0f, true);
		Bell->bCanInteract = false;
		Spawner->StartSpawning();
		bOpen = true;
		Cast<AInteractionHUD>(GetWorld()->GetFirstPlayerController()->GetHUD())->UpdateGameWidgetTime();
	}
}

void UDSManager::AllDishesWashed()
{
	Day += 1;
	Time = 0;
	Cast<AInteractionHUD>(GetWorld()->GetFirstPlayerController()->GetHUD())->UpdateGameWidgetTime();
	Bell->bCanInteract = true;
}




