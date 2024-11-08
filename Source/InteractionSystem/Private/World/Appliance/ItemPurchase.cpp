#include "World/Appliance/ItemPurchase.h"

#include "Data/ItemDataStructs.h"
#include "InteractionSystem/DSCharacter.h"
#include "Items/Dish.h"
#include "Items/Disinfectant.h"
#include "Items/Soap.h"
#include "Manager/DSManager.h"
#include "UserInterface/InteractionHUD.h"

AItemPurchase::AItemPurchase()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>("ItemMesh");
	ItemMesh->SetSimulatePhysics(false);
	SetRootComponent(ItemMesh);
	
}

void AItemPurchase::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<ADSCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	HUD = Cast<AInteractionHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	GameInstance = Cast<UDSManager>(GetGameInstance());
}

void AItemPurchase::BeginFocus()
{
	if (ItemMesh)
		ItemMesh->SetRenderCustomDepth(true);
}

void AItemPurchase::EndFocus()
{
	if (ItemMesh)
		ItemMesh->SetRenderCustomDepth(false);
}

void AItemPurchase::Interact(ADSCharacter* PlayerCharacter)
{
	if (PlayerCharacter)
	{
		if (!PlayerCharacter->GetIsHoldingItem())
			ItemPurchased();
		else
			UE_LOG(LogTemp, Warning, TEXT("HOLDING ITEM"));
	}
}

bool AItemPurchase::CanInteract()
{
	return !Cast<ADSCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn())->GetIsHoldingItem() && GameInstance->GetMoney() >= Price;
}

void AItemPurchase::DropItem(ADSCharacter* PlayerCharacter)
{
	IPickup::DropItem(PlayerCharacter);
}

FText AItemPurchase::GetInteractionHeader()
{
	return FText::Format(FText::FromString("64oz {0}\nCost - ${1}"), FText::FromString(ItemName), FText::AsNumber(Price));
}

FText AItemPurchase::GetInteractionText()
{
	return FText::Format(FText::FromString("Buy {0}"), FText::FromString(ItemName));
}

void AItemPurchase::ItemPurchased()
{
	Cast<AInteractionHUD>(GetWorld()->GetFirstPlayerController()->GetHUD())->HideInteractionWidget();
	EndFocus();
	
	FRotator RandomRotation;
	RandomRotation.Yaw = FMath::RandRange(1, 3) * 360.0f;
	RandomRotation.Pitch = FMath::RandRange(1, 3) * 360.0f;
	RandomRotation.Roll = FMath::RandRange(1, 3) * 360.0f;
	
	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.Instigator = GetInstigator();
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	if (!GetWorld())
		return;

	if (ItemType == EItemType::Disinfectant)
	{
		ADisinfectant* Item = GetWorld()->SpawnActor<ADisinfectant>(DisinfectantToSpawn, FVector::Zero(), RandomRotation, Params);
		Item->Interact(Player);
		GameInstance->ChangeMoney(-Price);
	}
	else if (ItemType == EItemType::Soap)
	{
		ASoap* Item = GetWorld()->SpawnActor<ASoap>(SoapToSpawn, FVector::Zero(), RandomRotation, Params);
		Item->Interact(Player);
		GameInstance->ChangeMoney(-Price);
	}
}




