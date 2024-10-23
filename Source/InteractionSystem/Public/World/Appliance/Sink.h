// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "Sink.generated.h"

UCLASS()
class INTERACTIONSYSTEM_API ASink : public AActor,  public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	ASink();

	UPROPERTY(EditAnywhere, Category = "Interaction Interface")
	bool bCanInteract;
	
	UPROPERTY(EditAnywhere, Category = "Mesh | Sink")
	UStaticMeshComponent* SinkMesh;

	UPROPERTY(EditAnywhere, Category = "Mesh | Water")
	UStaticMeshComponent* WaterMesh;
	
	UPROPERTY(EditInstanceOnly, Category = "Test Actor")
	FInteractableData InstanceInteractableData;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void BeginInteract() override;
	virtual void EndInteract() override;
	virtual void Interact(ADSCharacter* PlayerCharacter) override;
	virtual bool CanInteract() override;
};
