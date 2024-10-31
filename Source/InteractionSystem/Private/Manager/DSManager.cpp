#include "Manager/DSManager.h"

#include "UserInterface/InteractionHUD.h"

void UDSManager::Init()
{
	Super::Init();

	Day = 1;
	Money = 0;
}

void UDSManager::ChangeMoney(int amount)
{
	Money += amount;
	Cast<AInteractionHUD>(GetWorld()->GetFirstPlayerController()->GetHUD())->UpdateGameWidgetMoney(Money);
}




