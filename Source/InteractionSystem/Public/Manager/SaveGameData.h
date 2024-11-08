#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveGameData.generated.h"

enum class EItemType : uint8;
enum class EWasherState : uint8;
enum class ESanitizerState : uint8;

USTRUCT()
struct FItemSaveData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	int Volume;

	UPROPERTY()
	EItemType ItemType;
};

UCLASS()
class INTERACTIONSYSTEM_API USaveGameData : public USaveGame
{
	GENERATED_BODY()

public:
	USaveGameData();

	UPROPERTY(VisibleAnywhere, Category = "Save Data | Game Instance")
	int Money;

	UPROPERTY(VisibleAnywhere, Category = "Save Data | Game Instance")
	int Day;

	UPROPERTY(VisibleAnywhere, Category = "Save Data | Washer")
	EWasherState WasherState;

	UPROPERTY(VisibleAnywhere, Category = "Save Data | Washer")
	int SoapAmount;
	
	UPROPERTY(VisibleAnywhere, Category = "Save Data | Sanitizer")
	ESanitizerState SanitizerState;

	UPROPERTY(VisibleAnywhere, Category = "Save Data | Sanitizer")
	int DisinfectantAmount;

	UPROPERTY(VisibleAnywhere, Category = "Save Data | Item")
	TArray<FItemSaveData> SavedItems;
};
