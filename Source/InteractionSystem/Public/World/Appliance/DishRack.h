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
	
	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void Interact(ADSCharacter* PlayerCharacter) override;
	virtual bool CanInteract() override;
	virtual FText GetInteractionHeader() override;
	virtual FText GetInteractionText() override;
protected:
	
	UPROPERTY(EditAnywhere, Category = "Dish Rack", meta = (DisplayPriority = 0))
	UStaticMeshComponent* RackMesh;

	UPROPERTY(EditAnywhere, Category = "Dish Rack", meta = (DisplayPriority = 0))
	ADSCharacter* Player;
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};
