#include "DSCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Engine/Texture2D.h"
#include "Styling/SlateBrush.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Components/InventoryComponent.h"
#include "Data/ItemDataStructs.h"
#include "Interfaces/Pickup.h"
#include "UserInterface/InteractionHUD.h"
#include "WorldPartition/ContentBundle/ContentBundleLog.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

ADSCharacter::ADSCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	
	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	// Raycast for interaction
	InteractionCheckFrequency = 0.1f;
	InteractionCheckDistance = 250.0f;
	BaseEyeHeight = 56.0f;
	
	bCanMove = true;
	bIsHoldingItem = false;
	
}

void ADSCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetWorld()->TimeSince(InteractionData.LastInteractionCheckTime) > InteractionCheckFrequency)
		PerformInteractionCheck();
}

void ADSCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	HUD = Cast<AInteractionHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	ItemHeldType = EItemType::None;

	GetWorld()->GetFirstPlayerController()->CurrentMouseCursor = EMouseCursor::GrabHand;
}

void ADSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Toggle Menu
		EnhancedInputComponent->BindAction(ToggleMenuAction, ETriggerEvent::Started, this, &ADSCharacter::ToggleMenu);
		
		// Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ADSCharacter::Interact);
		//EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &ADSCharacter::EndInteract);

		// Drop
		EnhancedInputComponent->BindAction(DropAction, ETriggerEvent::Started, this, &ADSCharacter::DropHeldItem);
		
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADSCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADSCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ADSCharacter::PerformInteractionCheck()
{
	InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();

	FVector TraceStart{GetPawnViewLocation()};
	FVector TraceEnd{TraceStart + (GetViewRotation().Vector() * InteractionCheckDistance)};

	//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f, 0, 2.0f);
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	FHitResult TraceHit;
	
	if (GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
	{
		if (TraceHit.GetActor()->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
		{
			if (TraceHit.GetActor() != InteractionData.CurrentInteractable)
			{
				IInteractionInterface* TheInterface = Cast<IInteractionInterface>(TraceHit.GetActor());
				DisplayHeader(TheInterface->GetInteractionHeader());
					
				if (TheInterface->CanInteract())
				{
					FoundInteractable(TraceHit.GetActor());
					return;
				}
			}

			if (TraceHit.GetActor() == InteractionData.CurrentInteractable)
				return;
		}
		else
			HUD->HideInteractionHeader();
	}
	else
		HUD->HideInteractionHeader();
	
	NoInteractableFound();
}

void ADSCharacter::DisplayHeader(const FText& InteractableName) const
{
	HUD->UpdateInteractionHeader(InteractableName);
}

void ADSCharacter::FoundInteractable(AActor* NewInteractable)
{
	if (InteractionData.CurrentInteractable)
	{
		TargetInteractable = InteractionData.CurrentInteractable;
		TargetInteractable->EndFocus();
	}

	InteractionData.CurrentInteractable = NewInteractable;
	TargetInteractable = NewInteractable;
	
	HUD->UpdateInteractionWidget(Cast<IInteractionInterface>(NewInteractable)->GetInteractionText());
	
	TargetInteractable->BeginFocus();
}

void ADSCharacter::NoInteractableFound()
{
	if (InteractionData.CurrentInteractable)
	{
		if (IsValid(TargetInteractable.GetObject()))
			TargetInteractable->EndFocus();

		HUD->HideInteractionWidget();
		
		InteractionData.CurrentInteractable = nullptr;
		TargetInteractable = nullptr;
	}
}

void ADSCharacter::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT("E pressed"));
	PerformInteractionCheck();
	
	if (InteractionData.CurrentInteractable)
		if (IsValid(TargetInteractable.GetObject()))
			TargetInteractable->Interact(this);
}

void ADSCharacter::DropHeldItem()
{
	if (bIsHoldingItem)
	{
		UE_LOG(LogTemp, Warning, TEXT("Drop"));
		if (IPickup* ItemBeingHeld = Cast<IPickup>(HeldItem))
		{
			ItemBeingHeld->DropItem(this);
			HeldItem = nullptr;
			SetIsHoldingItem(false, EItemType::None);
		}
	}
}

void ADSCharacter::SetIsHoldingItem(bool toggle, EItemType itemType)
{
	if (toggle)
		ItemHeldType = itemType;
	else
		ItemHeldType = EItemType::None;

	bIsHoldingItem = toggle;
}

void ADSCharacter::UpdateInteractionWidget() const
{
	if (IsValid(TargetInteractable.GetObject()))
		HUD->UpdateInteractionWidget(TargetInteractable->GetInteractionText());
}

void ADSCharacter::ToggleMovement(bool toggle)
{
	if (toggle && !bCanMove)
		bCanMove = true;
	else if (!toggle && bCanMove)
		bCanMove = false;
}

void ADSCharacter::ToggleMenu()
{
	HUD->ToggleMenu();
}

void ADSCharacter::Move(const FInputActionValue& Value)
{
	if (!bCanMove)
		return;
	
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ADSCharacter::Look(const FInputActionValue& Value)
{
	if (!bCanMove)
		return;
	
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}


