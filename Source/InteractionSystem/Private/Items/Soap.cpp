#include "Items/Soap.h"
#include "Data/ItemDataStructs.h"
#include "InteractionSystem/DSCharacter.h"

ASoap::ASoap()
{
	PrimaryActorTick.bCanEverTick = true;

	SoapMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
	SoapMesh->SetSimulatePhysics(true);
	SetRootComponent(SoapMesh);
}

void ASoap::BeginPlay()
{
	Super::BeginPlay();
	
	SoapAmount = 640;
}

void ASoap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASoap::BeginFocus()
{
	if (SoapMesh)
		SoapMesh->SetRenderCustomDepth(true);
}

void ASoap::EndFocus()
{
	if (SoapMesh)
		SoapMesh->SetRenderCustomDepth(false);
}

void ASoap::Interact(ADSCharacter* PlayerCharacter)
{
	if (PlayerCharacter)
	{
		if (!PlayerCharacter->GetIsHoldingItem())
		{
			UE_LOG(LogTemp, Warning, TEXT("NOT HOLDING ITEM"));
			Cast<UPrimitiveComponent>(GetComponentByClass(UPrimitiveComponent::StaticClass()))->SetSimulatePhysics(false);
			SoapMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			AttachToComponent(PlayerCharacter->GetMesh1P(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("AttachSocket"));
			PlayerCharacter->SetIsHoldingItem(true, EItemType::Soap);
			PlayerCharacter->HeldItem = this;
		}
		else
			UE_LOG(LogTemp, Warning, TEXT("HOLDING ITEM"));
	}
}

bool ASoap::CanInteract()
{
	return !Cast<ADSCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn())->GetIsHoldingItem();
}

void ASoap::DropItem(ADSCharacter* PlayerCharacter)
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
	SoapMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

FText ASoap::GetInteractionHeader()
{
	return FText::Format(FText::FromString("Soap\n{0}oz / {1}oz"), FText::AsNumber(SoapAmount / 10.0f), FText::AsNumber(64));
}

FText ASoap::GetInteractionText()
{
	return FText::FromString("Press E To Pick Up Soap");
}
