#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DishSpawner.generated.h"

class ADish;

UCLASS()
class INTERACTIONSYSTEM_API ADishSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	ADishSpawner();
	
	UPROPERTY(VisibleAnywhere, Category = "Spawner", meta = (DisplayPriority = 0))
	TArray<ADish*> SpawnedDishes;
	
	void StartSpawning();
	void StopSpawning();
	void CheckAllDishesWashed();
	
protected:

	UPROPERTY(EditAnywhere, Category = "Spawner", meta = (DisplayPriority = 0))
	class UBoxComponent* SpawnVolume;

	UPROPERTY(EditAnywhere, Category = "Spawner", meta = (DisplayPriority = 0))
	TSubclassOf<class ADish> DishToSpawn;

	UPROPERTY(EditAnywhere, Category = "Spawner", meta = (DisplayPriority = 0))
	float MinSpawnDelay;

	UPROPERTY(EditAnywhere, Category = "Spawner", meta = (DisplayPriority = 0))
	float MaxSpawnDelay;
	
	float RandomSpawnDelay;
	FTimerHandle SpawnTimerHandle;
	FTimerHandle CheckDishesHandle;
	
	
	
	void SpawnDish();
	FVector GetRandomSpawnPoint();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};
