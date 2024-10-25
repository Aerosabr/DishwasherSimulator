#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "TrashCan.generated.h"

UCLASS()
class INTERACTIONSYSTEM_API ATrashCan : public AActor,  public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	ATrashCan();
	
	UPROPERTY(EditInstanceOnly, Category = "TrashCan | Interact Info")
	FInteractableData InstanceInteractableData;
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void BeginInteract() override;
	virtual void EndInteract() override;
	virtual void Interact(ADSCharacter* PlayerCharacter) override;
	virtual bool CanInteract() override;

protected:
	
	UPROPERTY(EditAnywhere, Category = "TrashCan | Interaction Interface")
	bool bCanInteract;
	
	UPROPERTY(EditAnywhere, Category = "TrashCan | Mesh")
	UStaticMeshComponent* TrashCanMesh;
	
};