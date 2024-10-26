#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "Interfaces/Pickup.h"
#include "Plate.generated.h"

enum class EDishState : uint8;

UCLASS()
class INTERACTIONSYSTEM_API APlate : public AActor,  public IInteractionInterface, public IPickup
{
	GENERATED_BODY()
	
public:	
	APlate();
	
	UPROPERTY(EditInstanceOnly, Category = "Plate")
	FInteractableData InstanceInteractableData;
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void BeginInteract() override;
	virtual void EndInteract() override;
	virtual void Interact(ADSCharacter* PlayerCharacter) override;
	virtual bool CanInteract() override;
	virtual void DropItem(ADSCharacter* PlayerCharacter) override;
	
protected:
	
	UPROPERTY(EditAnywhere, Category = "Plate")
	bool bCanInteract;

	UPROPERTY(VisibleAnywhere, Category = "Plate")
	EDishState DishState;
	
	UPROPERTY(EditAnywhere, Category = "Plate")
	UStaticMeshComponent* PlateMesh;
};
