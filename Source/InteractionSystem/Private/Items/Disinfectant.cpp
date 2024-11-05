#include "Items/Disinfectant.h"
#include "Data/ItemDataStructs.h"
#include "InteractionSystem/DSCharacter.h"

ADisinfectant::ADisinfectant()
{
	PrimaryActorTick.bCanEverTick = true;

	DisinfectantMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
	DisinfectantMesh->SetSimulatePhysics(true);
	SetRootComponent(DisinfectantMesh);
}

void ADisinfectant::BeginPlay()
{
	Super::BeginPlay();
	
	DisinfectantAmount = 1200;
}

void ADisinfectant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADisinfectant::BeginFocus()
{
	if (DisinfectantMesh)
		DisinfectantMesh->SetRenderCustomDepth(true);
}

void ADisinfectant::EndFocus()
{
	if (DisinfectantMesh)
		DisinfectantMesh->SetRenderCustomDepth(false);
}

void ADisinfectant::Interact(ADSCharacter* PlayerCharacter)
{
	if (PlayerCharacter)
	{
		if (!PlayerCharacter->GetIsHoldingItem())
		{
			UE_LOG(LogTemp, Warning, TEXT("NOT HOLDING ITEM"));
			Cast<UPrimitiveComponent>(GetComponentByClass(UPrimitiveComponent::StaticClass()))->SetSimulatePhysics(false);
			DisinfectantMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			AttachToComponent(PlayerCharacter->GetMesh1P(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("AttachSocket"));
			PlayerCharacter->SetIsHoldingItem(true, EItemType::Disinfectant);
			PlayerCharacter->HeldItem = this;
		}
		else
			UE_LOG(LogTemp, Warning, TEXT("HOLDING ITEM"));
	}
}

bool ADisinfectant::CanInteract()
{
	return !Cast<ADSCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn())->GetIsHoldingItem();
}

void ADisinfectant::DropItem(ADSCharacter* PlayerCharacter)
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
	DisinfectantMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

FText ADisinfectant::GetInteractionHeader()
{
	return FText::FromString("Disinfectant");
}

FText ADisinfectant::GetInteractionText()
{
	return FText::FromString("Press E To Pick Up Disinfectant");
}