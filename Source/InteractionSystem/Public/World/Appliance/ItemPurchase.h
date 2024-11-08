#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "Interfaces/Pickup.h"
#include "ItemPurchase.generated.h"

enum class EItemType : uint8;
class UDSManager;
class AInteractionHUD;

UCLASS()
class INTERACTIONSYSTEM_API AItemPurchase : public AActor,  public IInteractionInterface, public IPickup
{
	GENERATED_BODY()
	
public:	
	AItemPurchase();

	virtual void BeginPlay() override;
	
	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void Interact(ADSCharacter* PlayerCharacter) override;
	virtual bool CanInteract() override;
	virtual void DropItem(ADSCharacter* PlayerCharacter) override;
	
	virtual FText GetInteractionHeader() override;
	virtual FText GetInteractionText() override;
	
protected:

	UPROPERTY()
	UDSManager* GameInstance;
	
	UPROPERTY()
	AInteractionHUD* HUD;
	
	UPROPERTY(EditAnywhere, Category = "Item", meta = (DisplayPriority = 0))
	ADSCharacter* Player;

	UPROPERTY(EditAnywhere, Category = "Item", meta = (DisplayPriority = 0))
	int Price;

	UPROPERTY(EditAnywhere, Category = "Item", meta = (DisplayPriority = 0))
	FString ItemName;

	UPROPERTY(EditAnywhere, Category = "Item", meta = (DisplayPriority = 0))
	EItemType ItemType;
	
	UPROPERTY(EditAnywhere, Category = "Item", meta = (DisplayPriority = 0))
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(EditAnywhere, Category = "Item", meta = (DisplayPriority = 0))
	TSubclassOf<class ADisinfectant> DisinfectantToSpawn;

	UPROPERTY(EditAnywhere, Category = "Item", meta = (DisplayPriority = 0))
	TSubclassOf<class ASoap> SoapToSpawn;
	
	void ItemPurchased();
};
