#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "DishRack.generated.h"

UCLASS()
class INTERACTIONSYSTEM_API ADishRack : public AActor,  public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	ADishRack();

	UPROPERTY(EditInstanceOnly, Category = "Dish Rack")
	FInteractableData InstanceInteractableData;
	
	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void Interact(ADSCharacter* PlayerCharacter) override;
	virtual bool CanInteract() override;
protected:

	UPROPERTY(EditAnywhere, Category = "Dish Rack", meta = (DisplayPriority = 0))
	bool bCanInteract;

	UPROPERTY(EditAnywhere, Category = "Dish Rack", meta = (DisplayPriority = 0))
	UStaticMeshComponent* RackMesh;
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};
