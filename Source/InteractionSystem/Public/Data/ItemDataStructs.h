#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemDataStructs.generated.h"

UENUM()
enum class EDishState : uint8
{
	Dirty UMETA(DisplayName = "Dirty"),
	Scraped UMETA(DisplayName = "Scraped"),
	Washed UMETA(DisplayName = "Washed"),
	Rinsed UMETA(DisplayName = "Rinsed"),
	Sanitized UMETA(DisplayName = "Sanitized"),
};

UENUM()
enum class EItemType : uint8
{
	None UMETA(DisplayName = "None"),
	Soap UMETA(DisplayName = "Soap"),
	Disinfectant UMETA(DisplayName = "Disinfectant"),
	Dish UMETA(DisplayName = "Dish")
};

USTRUCT()
struct FItemAssetData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	TArray<UStaticMesh*> Meshes;

	UPROPERTY(EditAnywhere)
	int DebrisAmount;
};

USTRUCT()
struct FItemData : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FName ID;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemAssetData AssetData;
};