#include "Manager/DSManager.h"

#include "EngineUtils.h"
#include "Engine/Engine.h"
#include "TimerManager.h"
#include "Data/ItemDataStructs.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/SaveGameData.h"
#include "Items/DishSpawner.h"
#include "Items/Disinfectant.h"
#include "Items/Soap.h"
#include "UserInterface/InteractionHUD.h"
#include "World/Appliance/Sanitizer.h"
#include "World/Appliance/StartBell.h"
#include "World/Appliance/Washer.h"

void UDSManager::Init()
{
	Super::Init();
	
	Day = 1;
	Money = 0;
	Time = 0;
	bOpen = false;

	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,              // The timer handle
		this,                     // The object to call the function on
		&UDSManager::LoadGame,  // The function to call
		0.1f,                     // Delay in seconds
		false                     // Whether the timer should loop (set true to repeat)
	);
}

void UDSManager::SpawnStartItems() const
{
	const FVector SoapLocation(1195.0f, 1780.0f, 37.0f);
	const FVector DisinfectantLocation(1080.0f, 1765.0f, 40.0f);
	const FRotator SpawnRotation(0.0f, 0.0f, 0.0f);

	GetWorld()->SpawnActor<ASoap>(SoapToSpawn, SoapLocation, SpawnRotation);
	GetWorld()->SpawnActor<ADisinfectant>(DisinfectantToSpawn, DisinfectantLocation, SpawnRotation);
}

void UDSManager::OnTimerTick()
{
	Time += 3600;
	
	if (Time >= 79200)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		bOpen = false;
		Spawner->StopSpawning();
		Bell->bCanInteract = true;
	}
	
	HUD->UpdateGameWidgetTime();
}

void UDSManager::ChangeMoney(int amount)
{
	Money += amount;
	HUD->UpdateGameWidgetMoney();
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
		HUD->UpdateGameWidgetTime();
	}
}

void UDSManager::AllDishesWashed()
{
	Day += 1;
	Time = 0;
	HUD->UpdateGameWidgetTime();
	Bell->bCanInteract = true;
	SaveGame();
}

void UDSManager::CreateSaveFile()
{
	USaveGameData* dataToSave = Cast<USaveGameData>(UGameplayStatics::CreateSaveGameObject(USaveGameData::StaticClass()));
	UGameplayStatics::SaveGameToSlot(dataToSave, "Slot1", 0);
}

void UDSManager::SaveGame()
{
	UE_LOG(LogTemp, Warning, TEXT("Saving"));
	
	USaveGameData* dataToSave = Cast<USaveGameData>(UGameplayStatics::LoadGameFromSlot("Slot1", 0));

	if (dataToSave != nullptr)
	{
		dataToSave->Money = Money;
		dataToSave->Day = Day;
		
		for (const ASoap* SoapBottle : TActorRange<ASoap>(GetWorld()))
		{
			FItemSaveData Item;
			
			Item.Transform = SoapBottle->GetActorTransform();
			Item.Volume = SoapBottle->GetSoapAmount();
			Item.ItemType = EItemType::Soap;

			dataToSave->SavedItems.Add(Item);
		}
		for (const ADisinfectant* DisinfectantBottle : TActorRange<ADisinfectant>(GetWorld()))
		{
			FItemSaveData Item;
			
			Item.Transform = DisinfectantBottle->GetActorTransform();
			Item.Volume = DisinfectantBottle->GetDisinfectantAmount();
			Item.ItemType = EItemType::Disinfectant;

			dataToSave->SavedItems.Add(Item);
		}

		Washer->GetWasherState(dataToSave->WasherState, dataToSave->SoapAmount);
		Sanitizer->GetSanitizerState(dataToSave->SanitizerState, dataToSave->DisinfectantAmount);
		
		UGameplayStatics::SaveGameToSlot(dataToSave, "Slot1", 0);
	}
	else if (!UGameplayStatics::DoesSaveGameExist("Slot1", 0))
	{
		CreateSaveFile();
	}
}

void UDSManager::LoadGame()
{
	UE_LOG(LogTemp, Warning, TEXT("Loading"));
	HUD = Cast<AInteractionHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	USaveGameData* dataToLoad = Cast<USaveGameData>(UGameplayStatics::LoadGameFromSlot("Slot1", 0));

	if (dataToLoad != nullptr)
	{
		Money = dataToLoad->Money;
		Day = dataToLoad->Day;
		
		for (int i = 0; i < dataToLoad->SavedItems.Num(); i++)
		{
			FItemSaveData ItemData = dataToLoad->SavedItems[i];
			
			if (ItemData.ItemType == EItemType::Soap)
			{
				ASoap* SoapBottle = GetWorld()->SpawnActor<ASoap>(SoapToSpawn, ItemData.Transform);
				SoapBottle->SoapAmount = ItemData.Volume;
			}
			else
			{
				ADisinfectant* DisinfectantBottle = GetWorld()->SpawnActor<ADisinfectant>(DisinfectantToSpawn, ItemData.Transform);
				DisinfectantBottle->DisinfectantAmount = ItemData.Volume;
			}
		}

		Washer->SetWasherState(dataToLoad->WasherState, dataToLoad->SoapAmount);
		Sanitizer->SetSanitizerState(dataToLoad->SanitizerState, dataToLoad->DisinfectantAmount);
	}
	else if (!UGameplayStatics::DoesSaveGameExist("Slot1", 0))
	{
		CreateSaveFile();
		SpawnStartItems();
	}

	HUD->UpdateGameWidgetMoney();
	HUD->UpdateGameWidgetTime();
}




