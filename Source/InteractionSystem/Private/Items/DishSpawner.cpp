#include "Items/DishSpawner.h"

#include "Components/BoxComponent.h"
#include "Items/Dish.h"
#include "Kismet/KismetMathLibrary.h"
#include "Manager/DSManager.h"

ADishSpawner::ADishSpawner()
{
	PrimaryActorTick.bCanEverTick = true;

	SpawnVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Spawner"));

	MinSpawnDelay = 2.5f;
	MaxSpawnDelay = 5.0f;
}

void ADishSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (UDSManager* GameInstance = Cast<UDSManager>(GetGameInstance()))
		GameInstance->SetSpawner(this);
	
	RandomSpawnDelay = FMath::RandRange(MinSpawnDelay, MaxSpawnDelay);
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ADishSpawner::SpawnDish, RandomSpawnDelay, false);
}

void ADishSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADishSpawner::StartSpawning()
{
	RandomSpawnDelay = FMath::RandRange(MinSpawnDelay, MaxSpawnDelay);
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ADishSpawner::SpawnDish, RandomSpawnDelay, false);
}

void ADishSpawner::StopSpawning()
{
	GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
	CheckAllDishesWashed();
}

void ADishSpawner::CheckAllDishesWashed()
{
	if (SpawnedDishes.Num() == 0)
	{
		if (UDSManager* GameInstance = Cast<UDSManager>(GetGameInstance()))
			GameInstance->AllDishesWashed();
		UE_LOG(LogTemp, Warning, TEXT("All dishes washed"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("There are %d dishes remaining"), SpawnedDishes.Num());
		GetWorld()->GetTimerManager().SetTimer(CheckDishesHandle, this, &ADishSpawner::CheckAllDishesWashed, 1.0f, false);
	}
}

void ADishSpawner::SpawnDish()
{
	if (UDSManager* GameInstance = Cast<UDSManager>(GetGameInstance()))
		if (!GameInstance->GetOpen())
			return;
	
	FRotator RandomRotation;
	RandomRotation.Yaw = FMath::RandRange(1, 3) * 360.0f;
	RandomRotation.Pitch = FMath::RandRange(1, 3) * 360.0f;
	RandomRotation.Roll = FMath::RandRange(1, 3) * 360.0f;
	
	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.Instigator = GetInstigator();
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	if (!GetWorld())
		return;

	ADish* DishSpawned = GetWorld()->SpawnActor<ADish>(DishToSpawn, GetRandomSpawnPoint(), RandomRotation, Params);
	SpawnedDishes.Add(DishSpawned);
	
	RandomSpawnDelay = FMath::RandRange(MinSpawnDelay, MaxSpawnDelay);
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ADishSpawner::SpawnDish, RandomSpawnDelay, false);
}

FVector ADishSpawner::GetRandomSpawnPoint()
{
	const FVector SpawnOrigin = SpawnVolume->Bounds.Origin;
	const FVector SpawnLimit = SpawnVolume->Bounds.BoxExtent;

	return UKismetMathLibrary::RandomPointInBoundingBox(SpawnOrigin, SpawnLimit);
}
