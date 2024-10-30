#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "Rinser.generated.h"

class ADish;

UCLASS()
class INTERACTIONSYSTEM_API ARinser : public AActor,  public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	ARinser();
	
	UPROPERTY(EditInstanceOnly, Category = "Rinser", meta = (DisplayPriority = 0))
	FInteractableData InstanceInteractableData;
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void Interact(ADSCharacter* PlayerCharacter) override;
	virtual bool CanInteract() override;

protected:
	
	UPROPERTY(EditAnywhere, Category = "Rinser", meta = (DisplayPriority = 0))
	bool bCanInteract;
	
	UPROPERTY(EditAnywhere, Category = "Rinser", meta = (DisplayPriority = 0))
	UStaticMeshComponent* RinserMesh;

	// Scrubbing variables
	UPROPERTY(EditAnywhere, Category = "Rinser", meta = (DisplayPriority = 0))
	bool bIsScrubbing;

	UPROPERTY(EditAnywhere, Category = "Rinser", meta = (DisplayPriority = 0))
	ADSCharacter* Player;
	FVector LastMousePosition;
	FVector RotationCenter;
	float CumulativeDistance;
	float DistanceThreshold;
	
	// Scrubbing functions
	void StartRinsing();
	void CalculateDistance(FVector MousePosition);
	FVector GetCurrentMousePosition();
	
	void InteractedWithDish(ADSCharacter* PlayerCharacter);
};
