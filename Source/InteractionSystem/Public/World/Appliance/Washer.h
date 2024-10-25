#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "Washer.generated.h"

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
	
	UPROPERTY(EditInstanceOnly, Category = "Washer")
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
	
	UPROPERTY(VisibleAnywhere, Category = "Washer")
	EWasherState WasherState;
	
	UPROPERTY(EditAnywhere, Category = "Washer")
	bool bCanInteract;

	UPROPERTY(EditAnywhere, Category = "Washer")
	TArray<UMaterialInterface*> Materials;
	
	UPROPERTY(EditAnywhere, Category = "Washer")
	UStaticMeshComponent* WasherMesh;

	void SetWaterMesh();
};

