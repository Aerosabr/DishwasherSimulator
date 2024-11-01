#include "World/Appliance/StartBell.h"

#include "Manager/DSManager.h"

AStartBell::AStartBell()
{
	PrimaryActorTick.bCanEverTick = true;

	BellMesh = CreateDefaultSubobject<UStaticMeshComponent>("SanitizerMesh");
	SetRootComponent(BellMesh);
	
	bCanInteract = true;
}

void AStartBell::BeginPlay()
{
	Super::BeginPlay();
	
}

void AStartBell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStartBell::BeginFocus()
{
	if (BellMesh)
		BellMesh->SetRenderCustomDepth(true);
}

void AStartBell::EndFocus()
{
	if (BellMesh)
		BellMesh->SetRenderCustomDepth(false);
}

void AStartBell::Interact(ADSCharacter* PlayerCharacter)
{
	if (UDSManager* GameInstance = Cast<UDSManager>(GetGameInstance()))
		GameInstance->ToggleTimer();
}

bool AStartBell::CanInteract()
{
	return bCanInteract;
}