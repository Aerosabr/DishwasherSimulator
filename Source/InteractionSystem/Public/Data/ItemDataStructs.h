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
enum class EItemQuality : uint8
{
	Common UMETA(DisplayName = "Common"),
	Uncommon UMETA(DisplayName = "Uncommon"),
	Rare UMETA(DisplayName = "Rare"),
	Epic UMETA(DisplayName = "Epic"),
	Legendary UMETA(DisplayName = "Legendary"),
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
struct FItemStatistics
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere)
	float FaucetPower;

	UPROPERTY(EditAnywhere)
	float FaucetWaterUsage;

	UPROPERTY(EditAnywhere)
	float SoapPower;
	
	UPROPERTY(EditAnywhere)
	int SoapUsages;

	UPROPERTY(EditAnywhere)
	float BuyValue;
	
	UPROPERTY(EditAnywhere)
	float SellValue;
};

USTRUCT()
struct FItemTextData
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere)
	FText Name;
	
	UPROPERTY(EditAnywhere)
	FText Description;
	
	UPROPERTY(EditAnywhere)
	FText InteractionText;
	
	UPROPERTY(EditAnywhere)
	FText UsageText;
};

USTRUCT()
struct FItemAssetData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	TArray<UStaticMesh*> Meshes;
};

USTRUCT()
struct FItemData : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FName ID;
	
	UPROPERTY(EditAnywhere, Category = "Item Data")
	EItemType ItemType;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	EItemQuality ItemQuality;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemStatistics ItemStatistics;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemTextData TextData;

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemAssetData AssetData;
};