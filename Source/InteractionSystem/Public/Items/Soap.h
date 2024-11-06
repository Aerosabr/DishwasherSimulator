#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "Interfaces/Pickup.h"
#include "Soap.generated.h"

UCLASS()
class INTERACTIONSYSTEM_API ASoap : public AActor,  public IInteractionInterface, public IPickup
{
	GENERATED_BODY()
	
public:	
	ASoap();

	UPROPERTY(EditAnywhere, Category = "Soap")
	int SoapAmount;
	
	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void Interact(ADSCharacter* PlayerCharacter) override;
	virtual bool CanInteract() override;
	virtual void DropItem(ADSCharacter* PlayerCharacter) override;
	
	virtual FText GetInteractionHeader() override;
	virtual FText GetInteractionText() override;
	FORCEINLINE int GetSoapAmount() const {	return SoapAmount; };
	
protected:
	
	UPROPERTY(EditAnywhere, Category = "Soap")
	UStaticMeshComponent* SoapMesh;
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
};
