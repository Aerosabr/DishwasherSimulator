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

	UPROPERTY(VisibleAnywhere, Category = "Dish")
	int DebrisAmount;

	UPROPERTY(VisibleAnywhere, Category = "Dish")
	int Value;
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void Interact(ADSCharacter* PlayerCharacter) override;
	virtual bool CanInteract() override;
	virtual void DropItem(ADSCharacter* PlayerCharacter) override;
	virtual FText GetInteractionHeader() override;
	virtual FText GetInteractionText() override;
	FORCEINLINE EDishState GetDishState() const { return DishState; };
	void ProgressDishState();
	void SetDishMesh();
	
protected:
	
	UPROPERTY(VisibleAnywhere, Category = "Dish")
	EDishState DishState;
	
	UPROPERTY(EditAnywhere, Category = "Dish")
	UStaticMeshComponent* DishMesh;

	UPROPERTY(EditAnywhere, Category = "Dish")
	TArray<UStaticMesh*> DishMeshes;

	UPROPERTY(EditAnywhere, Category = "Dish")
	TArray<UNiagaraSystem*> Particles;

	UPROPERTY(EditAnywhere, Category = "Dish")
	UNiagaraComponent* Particle;
	
	UPROPERTY(EditAnywhere, Category = "Dish")
	FDataTableRowHandle ItemRowHandle;
	
};
