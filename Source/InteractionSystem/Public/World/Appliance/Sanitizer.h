#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "Sanitizer.generated.h"

class ADish;

UENUM()
enum class ESanitizerState : uint8
{
	Default UMETA(DisplayName = "Default"),
	Sanitized UMETA(DisplayName = "Sanitized"),
};

UCLASS()
class INTERACTIONSYSTEM_API ASanitizer : public AActor,  public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	ASanitizer();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void Interact(ADSCharacter* PlayerCharacter) override;
	virtual bool CanInteract() override;
	virtual FText GetInteractionHeader() override;
	virtual FText GetInteractionText() override;
	
protected:
	UPROPERTY(VisibleAnywhere, Category = "Sanitizer", meta = (DisplayPriority = 0))
	ESanitizerState SanitizerState;
	
	UPROPERTY(EditAnywhere, Category = "Sanitizer", meta = (DisplayPriority = 0))
	int DisinfectantAmount;

	UPROPERTY(EditAnywhere, Category = "Sanitizer", meta = (DisplayPriority = 0))
	TArray<UMaterialInterface*> Materials;
	
	UPROPERTY(EditAnywhere, Category = "Sanitizer", meta = (DisplayPriority = 0))
	UStaticMeshComponent* SanitizerMesh;

	// Scrubbing variables
	UPROPERTY(EditAnywhere, Category = "Sanitizer", meta = (DisplayPriority = 0))
	bool bIsScrubbing;
	
	UPROPERTY(EditAnywhere, Category = "Sanitizer", meta = (DisplayPriority = 0))
	ADSCharacter* Player;
	FVector LastMousePosition;
	FVector RotationCenter;
	float CumulativeDistance;
	float DistanceThreshold;
	
	// Scrubbing functions
	void StartSanitizing();
	void CalculateDistance(FVector MousePosition);
	FVector GetCurrentMousePosition();
	
	void SetWaterMesh();
	void InteractedWithDish();
	void InteractedWithDisinfectant();
};
