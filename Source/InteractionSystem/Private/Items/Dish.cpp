#include "Items/Dish.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Data/ItemDataStructs.h"
#include "InteractionSystem/DSCharacter.h"
#include "Kismet/KismetMathLibrary.h"

ADish::ADish()
{
	PrimaryActorTick.bCanEverTick = true;

	DishMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
	DishMesh->SetSimulatePhysics(true);
	SetRootComponent(DishMesh);
	Particle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraParticleComponent"));

}

void ADish::BeginPlay()
{
	Super::BeginPlay();

	DishState = EDishState::Dirty;

	DebrisAmount = UKismetMathLibrary::RandomIntegerInRange(20, 40);
	Value = UKismetMathLibrary::RandomIntegerInRange(3, 5);
}

void ADish::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Particle && RootComponent)
	{
		Particle->SetWorldLocation(RootComponent->GetComponentLocation()); 
		Particle->SetWorldRotation(RootComponent->GetComponentRotation()); 
	}
}

void ADish::BeginFocus()
{
	if (DishMesh)
		DishMesh->SetRenderCustomDepth(true);
}

void ADish::EndFocus()
{
	if (DishMesh)
		DishMesh->SetRenderCustomDepth(false);
}

void ADish::Interact(ADSCharacter* PlayerCharacter)
{
	if (PlayerCharacter)
	{
		if (!PlayerCharacter->GetIsHoldingItem())
		{
			UE_LOG(LogTemp, Warning, TEXT("NOT HOLDING ITEM"));
			Cast<UPrimitiveComponent>(GetComponentByClass(UPrimitiveComponent::StaticClass()))->SetSimulatePhysics(false);
			DishMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			AttachToComponent(PlayerCharacter->GetMesh1P(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("AttachSocket"));
			PlayerCharacter->SetIsHoldingItem(true, EItemType::Dish);
			PlayerCharacter->HeldItem = this;
		}
		else
			UE_LOG(LogTemp, Warning, TEXT("HOLDING ITEM"));
	}
}

bool ADish::CanInteract()
{
	return !Cast<ADSCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn())->GetIsHoldingItem();
}

void ADish::DropItem(ADSCharacter* PlayerCharacter)
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	
	FVector ViewLocation;
	FRotator ViewRotation;
	if (PlayerCharacter->GetController())
		PlayerCharacter->GetController()->GetPlayerViewPoint(ViewLocation, ViewRotation);
	
	FVector EndLocation = ViewLocation + (ViewRotation.Vector() * 500.0f); 

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); 
	QueryParams.AddIgnoredActor(PlayerCharacter); 
	
	if (GetWorld()->LineTraceSingleByChannel(HitResult, ViewLocation, EndLocation, ECC_Visibility, QueryParams))
	{
		const FVector SpawnLocation = HitResult.Location;
		const FTransform SpawnTransform(PlayerCharacter->GetActorRotation(), SpawnLocation);

		SetActorTransform(SpawnTransform);
	}
	else
	{
		const FVector FeetLocation = PlayerCharacter->GetActorLocation() - FVector(0, 0, PlayerCharacter->GetMesh()->GetComponentLocation().Z);
		const FVector SpawnLocation = FeetLocation + (PlayerCharacter->GetActorForwardVector() * 50.0f);
		const FTransform SpawnTransform(PlayerCharacter->GetActorRotation(), SpawnLocation);

		SetActorTransform(SpawnTransform);
	}
	
	Cast<UPrimitiveComponent>(GetComponentByClass(UPrimitiveComponent::StaticClass()))->SetSimulatePhysics(true);
	DishMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

FText ADish::GetInteractionHeader()
{
	return FText::FromString("Dish");
}

FText ADish::GetInteractionText()
{
	return FText::FromString("Press E To Pick Up Dish");
}

void ADish::ProgressDishState()
{
	switch (DishState)
	{
		case EDishState::Dirty:
			DishState = EDishState::Scraped;
			SetDishMesh();
			break;
		case EDishState::Scraped:
			DishState = EDishState::Washed;
			SetDishMesh();
			Particle->SetAsset(Particles[0]); 
			Particle->RegisterComponent(); 
			Particle->Activate();
			Particle->SetVisibility(true);
			break;
		case EDishState::Washed:
			DishState = EDishState::Rinsed;
			SetDishMesh();
			Particle->Deactivate();
			break;
		case EDishState::Rinsed:
			DishState = EDishState::Sanitized;
			SetDishMesh();
			Particle->SetAsset(Particles[1]); 
			Particle->RegisterComponent(); 
			Particle->Activate();
			break;
		case EDishState::Sanitized:
			
			break;
	}
}

void ADish::SetDishMesh()
{
	switch (DishState)
	{
	case EDishState::Dirty:
		DishMesh->SetStaticMesh(DishMeshes[0]);
		break;
	case EDishState::Scraped:
		DishMesh->SetStaticMesh(DishMeshes[1]);
		break;
	case EDishState::Washed:
		DishMesh->SetStaticMesh(DishMeshes[2]);
		
		break;
	case EDishState::Rinsed:
		DishMesh->SetStaticMesh(DishMeshes[3]);
		break;
	case EDishState::Sanitized:
		DishMesh->SetStaticMesh(DishMeshes[4]);
		break;
	default:
		DishMesh->SetStaticMesh(DishMeshes[0]);
		break;
	}
	
}



