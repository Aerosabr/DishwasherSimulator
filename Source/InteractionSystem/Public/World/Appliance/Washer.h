#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "Washer.generated.h"

class ADish;

UENUM()
enum class EWasherState : uint8
{
	Default UMETA(DisplayName = "Default"),
	Soap UMETA(DisplayName = "Soap"),
	Dirty UMETA(DisplayName = "Dirty")
};

UCLASS()
class INTERACTIONSYSTEM_API AWasher : public AActor,  public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	AWasher();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void Interact(ADSCharacter* PlayerCharacter) override;
	virtual bool CanInteract() override;
	virtual FText GetInteractionHeader() override;
	virtual FText GetInteractionText() override;
	
protected:
	
	UPROPERTY(VisibleAnywhere, Category = "Washer", meta = (DisplayPriority = 0))
	EWasherState WasherState;
	
	UPROPERTY(EditAnywhere, Category = "Washer", meta = (DisplayPriority = 0))
	int SoapAmount;

	UPROPERTY(EditAnywhere, Category = "Washer", meta = (DisplayPriority = 0))
	TArray<UMaterialInterface*> Materials;
	
	UPROPERTY(EditAnywhere, Category = "Washer", meta = (DisplayPriority = 0))
	UStaticMeshComponent* WasherMesh;
	
	UPROPERTY(EditAnywhere, Category = "Washer", meta = (DisplayPriority = 0))
	bool bIsScrubbing;
	
	UPROPERTY(EditAnywhere, Category = "Washer", meta = (DisplayPriority = 0))
	ADSCharacter* Player;
	
	FVector LastMousePosition;
	FVector RotationCenter;
	float CumulativeDistance;
	float DistanceThreshold;
	
	void StartWashing();
	void CalculateDistance(FVector MousePosition);
	FVector GetCurrentMousePosition();
	
	void SetWaterMesh();

	void InteractedWithDish(ADSCharacter* PlayerCharacter);
};

