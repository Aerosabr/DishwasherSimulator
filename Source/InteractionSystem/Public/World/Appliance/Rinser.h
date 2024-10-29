#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "Rinser.generated.h"

UCLASS()
class INTERACTIONSYSTEM_API ARinser : public AActor,  public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	ARinser();
	
	UPROPERTY(EditInstanceOnly, Category = "Rinser")
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
	
	UPROPERTY(EditAnywhere, Category = "Rinser")
	bool bCanInteract;
	
	UPROPERTY(EditAnywhere, Category = "Rinser")
	UStaticMeshComponent* RinserMesh;

	static void InteractedWithDish(const ADSCharacter* PlayerCharacter);
};
