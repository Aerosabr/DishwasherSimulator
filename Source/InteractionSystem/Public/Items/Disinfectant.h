#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "Interfaces/Pickup.h"
#include "Disinfectant.generated.h"

UCLASS()
class INTERACTIONSYSTEM_API ADisinfectant : public AActor,  public IInteractionInterface, public IPickup
{
	GENERATED_BODY()
	
public:	
	ADisinfectant();

	UPROPERTY(EditAnywhere, Category = "Disinfectant")
	int DisinfectantAmount;
	
	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void Interact(ADSCharacter* PlayerCharacter) override;
	virtual bool CanInteract() override;
	virtual void DropItem(ADSCharacter* PlayerCharacter) override;
	virtual FText GetInteractionHeader() override;
	virtual FText GetInteractionText() override;
	FORCEINLINE int GetDisinfectantAmount() const { return DisinfectantAmount; };

protected:
	
	UPROPERTY(EditAnywhere, Category = "Disinfectant")
	UStaticMeshComponent* DisinfectantMesh;
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
};
