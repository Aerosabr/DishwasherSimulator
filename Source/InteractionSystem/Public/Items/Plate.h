#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "Plate.generated.h"

UCLASS()
class INTERACTIONSYSTEM_API APlate : public AActor,  public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	APlate();
	
	UPROPERTY(EditInstanceOnly, Category = "Test Actor")
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
	
	UPROPERTY(EditAnywhere, Category = "Interaction Interface")
	bool bCanInteract;
	
	UPROPERTY(EditAnywhere, Category = "Mesh | Plate")
	UStaticMeshComponent* PlateMesh;
};
