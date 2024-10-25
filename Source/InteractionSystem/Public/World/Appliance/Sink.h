#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "Sink.generated.h"

UENUM()
enum class EWaterState : uint8
{
	Default UMETA(DisplayName = "Default"),
	Soap UMETA(DisplayName = "Soap"),
	Dirty UMETA(DisplayName = "Dirty")
};

UCLASS()
class INTERACTIONSYSTEM_API ASink : public AActor,  public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	ASink();
	
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
	UPROPERTY(VisibleAnywhere, Category = "Water State")
	EWaterState WaterState;
	
	UPROPERTY(EditAnywhere, Category = "Interaction Interface")
	bool bCanInteract;

	UPROPERTY(EditAnywhere, Category = "Mesh | Material")
	TArray<UMaterialInterface*> Materials;
	
	UPROPERTY(EditAnywhere, Category = "Mesh | Sink")
	UStaticMeshComponent* SinkMesh;

	UPROPERTY(EditAnywhere, Category = "Mesh | Water")
	UStaticMeshComponent* WaterMesh;

	void SetWaterMesh();
};
