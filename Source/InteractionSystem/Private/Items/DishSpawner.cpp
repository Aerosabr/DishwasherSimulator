#include "Items/DishSpawner.h"

#include "Components/BoxComponent.h"
#include "Items/Dish.h"
#include "Kismet/KismetMathLibrary.h"

ADishSpawner::ADishSpawner()
{
	PrimaryActorTick.bCanEverTick = true;

	SpawnVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Spawner"));

	MinSpawnDelay = 0.5f;
	MaxSpawnDelay = 5.0f;
}

void ADishSpawner::BeginPlay()
{
	Super::BeginPlay();

	RandomSpawnDelay = FMath::RandRange(MinSpawnDelay, MaxSpawnDelay);
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ADishSpawner::SpawnDish, RandomSpawnDelay, false);
}

void ADishSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADishSpawner::SpawnDish()
{
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

	RandomSpawnDelay = FMath::RandRange(MinSpawnDelay, MaxSpawnDelay);
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ADishSpawner::SpawnDish, RandomSpawnDelay, false);
}

FVector ADishSpawner::GetRandomSpawnPoint()
{
	const FVector SpawnOrigin = SpawnVolume->Bounds.Origin;
	const FVector SpawnLimit = SpawnVolume->Bounds.BoxExtent;

	return UKismetMathLibrary::RandomPointInBoundingBox(SpawnOrigin, SpawnLimit);
}
