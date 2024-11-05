#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "TrashCan.generated.h"

class ADish;

UCLASS()
class INTERACTIONSYSTEM_API ATrashCan : public AActor,  public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	ATrashCan();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void Interact(ADSCharacter* PlayerCharacter) override;
	virtual bool CanInteract() override;
	virtual FText GetInteractionHeader() override;
	virtual FText GetInteractionText() override;
protected:
	
	UPROPERTY(EditAnywhere, Category = "TrashCan", meta = (DisplayPriority = 0))
	UStaticMeshComponent* TrashCanMesh;
	
	UPROPERTY(EditAnywhere, Category = "TrashCan", meta = (DisplayPriority = 0))
	bool bIsScrubbing;

	UPROPERTY(EditAnywhere, Category = "TrashCan", meta = (DisplayPriority = 0))
	ADSCharacter* Player;
	
	FVector LastMousePosition;
	FVector RotationCenter;
	float CumulativeDistance;
	float DistanceThreshold;
	
	void StartScraping();
	void CalculateDistance(FVector MousePosition);
	FVector GetCurrentMousePosition();
	
	void InteractedWithDish(ADSCharacter* PlayerCharacter);
};