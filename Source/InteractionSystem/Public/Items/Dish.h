#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "Interfaces/Pickup.h"
#include "Dish.generated.h"
class UNiagaraSystem;
class UNiagaraComponent;
enum class EDishState : uint8;

UCLASS()
class INTERACTIONSYSTEM_API ADish : public AActor,  public IInteractionInterface, public IPickup
{
	GENERATED_BODY()
	
public:	
	ADish();
	
	UPROPERTY(EditInstanceOnly, Category = "Plate")
	FInteractableData InstanceInteractableData;
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void Interact(ADSCharacter* PlayerCharacter) override;
	virtual bool CanInteract() override;
	virtual void DropItem(ADSCharacter* PlayerCharacter) override;
	FORCEINLINE EDishState GetDishState() const { return DishState; };
	void ProgressDishState();
	void SetDishMesh();
	
protected:
	
	UPROPERTY(EditAnywhere, Category = "Plate")
	bool bCanInteract;

	UPROPERTY(VisibleAnywhere, Category = "Plate")
	EDishState DishState;
	
	UPROPERTY(EditAnywhere, Category = "Plate")
	UStaticMeshComponent* DishMesh;

	UPROPERTY(EditAnywhere, Category = "Plate")
	TArray<UStaticMesh*> DishMeshes;

	UPROPERTY(EditAnywhere, Category = "Plate")
	TArray<UNiagaraSystem*> Particles;

	UPROPERTY(EditAnywhere, Category = "Plate")
	UNiagaraComponent* Particle;
	
	UPROPERTY(EditAnywhere, Category = "Plate")
	FDataTableRowHandle ItemRowHandle;
	
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
};
