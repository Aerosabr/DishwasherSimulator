#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "Sanitizer.generated.h"

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
	
	UPROPERTY(EditInstanceOnly, Category = "Sanitizer")
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
	UPROPERTY(VisibleAnywhere, Category = "Sanitizer")
	ESanitizerState SanitizerState;
	
	UPROPERTY(EditAnywhere, Category = "Sanitizer")
	bool bCanInteract;

	UPROPERTY(EditAnywhere, Category = "Sanitizer")
	TArray<UMaterialInterface*> Materials;
	
	UPROPERTY(EditAnywhere, Category = "Sanitizer")
	UStaticMeshComponent* SanitizerMesh;

	void SetWaterMesh();
};
