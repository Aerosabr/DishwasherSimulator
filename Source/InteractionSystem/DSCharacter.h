#pragma once

#include "CoreMinimal.h"
#include "TimerManager.h"
#include "GameFramework/Character.h"
#include "Interfaces/InteractionInterface.h"
#include "Logging/LogMacros.h"
#include "DSCharacter.generated.h"

enum class EItemType : uint8;
class UInventoryComponent;
class AInteractionHUD;
class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

USTRUCT()
struct FInteractionData
{
	GENERATED_USTRUCT_BODY()

	FInteractionData() : CurrentInteractable(nullptr), LastInteractionCheckTime(0.0f)
	{
		
	};
	
	UPROPERTY()
	AActor* CurrentInteractable;

	UPROPERTY()
	float LastInteractionCheckTime;
};

UCLASS(config=Game)
class ADSCharacter : public ACharacter
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;
	
#pragma region Inputs
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DropAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ToggleMenuAction;
#pragma endregion
	
public:
	UPROPERTY(VisibleAnywhere, Category = "Character")
	AActor* HeldItem;
	
	ADSCharacter();
	
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
	FORCEINLINE bool GetIsHoldingItem() const { return bIsHoldingItem; };
	FORCEINLINE EItemType GetHeldItemType() const { return ItemHeldType; };
	void SetIsHoldingItem(bool toggle, EItemType itemType);
	void UpdateInteractionWidget() const;
	void ToggleMovement(bool toggle);

protected:

	UPROPERTY()
	AInteractionHUD* HUD;
	
	UPROPERTY(VisibleAnywhere, Category = "Character", meta = (DisplayPriority = 0))
	TScriptInterface<IInteractionInterface> TargetInteractable;

	UPROPERTY(VisibleAnywhere, Category = "Character", meta = (DisplayPriority = 0))
	EItemType ItemHeldType;
	
	float InteractionCheckFrequency;
	float InteractionCheckDistance;

	UPROPERTY(VisibleAnywhere, Category = "Character", meta = (DisplayPriority = 0))
	bool bIsHoldingItem;

	UPROPERTY(VisibleAnywhere, Category = "Character", meta = (DisplayPriority = 0))
	bool bCanMove;

	FInteractionData InteractionData;
	
	void PerformInteractionCheck();
	void DisplayHeader(const FText& InteractableName) const;
	void FoundInteractable(AActor* NewInteractable);
	void NoInteractableFound();
	void Interact();
	void DropHeldItem();
	
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

	void ToggleMenu();
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	
};

