#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "StartBell.generated.h"

UCLASS()
class INTERACTIONSYSTEM_API AStartBell : public AActor,  public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	AStartBell();

	UPROPERTY(EditInstanceOnly, Category = "StartBell", meta = (DisplayPriority = 0))
	FInteractableData InstanceInteractableData;

	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void Interact(ADSCharacter* PlayerCharacter) override;
	virtual bool CanInteract() override;
	
protected:

	UPROPERTY(EditAnywhere, Category = "StartBell", meta = (DisplayPriority = 0))
	bool bCanInteract;
	
	UPROPERTY(EditAnywhere, Category = "StartBell", meta = (DisplayPriority = 0))
	UStaticMeshComponent* BellMesh;
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};
