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
	
	UPROPERTY(EditAnywhere, Category = "StartBell", meta = (DisplayPriority = 0))
	bool bCanInteract;
	
	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void Interact(ADSCharacter* PlayerCharacter) override;
	virtual bool CanInteract() override;
	virtual FText GetInteractionHeader() override;
	virtual FText GetInteractionText() override;
protected:
	
	UPROPERTY(EditAnywhere, Category = "StartBell", meta = (DisplayPriority = 0))
	UStaticMeshComponent* BellMesh;
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};
