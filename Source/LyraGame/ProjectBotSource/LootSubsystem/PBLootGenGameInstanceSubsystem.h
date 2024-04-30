// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AsyncMixin.h"
#include "GameplayTagContainer.h"
#include "ProjectBotSource/Inventory/PBInventoryEnums.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PBLootGenGameInstanceSubsystem.generated.h"

enum class EPBItemQuality : uint8;
class UPBItemDefinition;
class UPBInventoryItemInstance;

DECLARE_LOG_CATEGORY_EXTERN(LogPBLootSubsystem, Log, All);

DECLARE_DYNAMIC_DELEGATE_OneParam(FPBItemInstanceGenerated, UPBInventoryItemInstance*, GeneratedInstance);

USTRUCT(BlueprintType)
struct FPBTextRow : public FTableRowBase
{
	GENERATED_BODY()

public:

	FPBTextRow()
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Text;
};

/**
 * 
 */
UCLASS()
class LYRAGAME_API UPBLootGenGameInstanceSubsystem : public UGameInstanceSubsystem, public FAsyncMixin
{
	GENERATED_BODY()

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	/*UFUNCTION(BlueprintCallable, Category = "LootGenSystem")
	UPBInventoryItemInstance* GenerateItemInstance();

	UFUNCTION(BlueprintCallable, Category = "LootGenSystem")
	UPBInventoryItemInstance* GenerateItemInstanceFromPool(UPBItemPool* Pool);*/

	FName GetRandomAdjectiveRowKey();

	int GenerateNumMods(EPBItemQuality InQuality);

	FText GetAdjectiveForKey(FName InKey);

	UFUNCTION(BlueprintCallable, Category = "LootGenSystem")
	UPBInventoryItemInstance* GenerateItemInstance(UPBItemDefinition* ItemDef);

	UFUNCTION(BlueprintCallable, Category = "LootGenSystem")
	void GetCompatibleModAssetsForItemInstance(UPBInventoryItemInstance* ItemInstance, TArray<FAssetData>& OutDataArray);

	UFUNCTION(BlueprintCallable, Category = "LootGenSystem", meta = (AutoCreateRefTerm = "Delegate"))
	void GenerateItemInstanceFromSoftDel(TSoftObjectPtr<UPBItemDefinition> ItemDefRef, const FPBItemInstanceGenerated& Delegate);

protected:
	UPROPERTY()
	TObjectPtr<UDataTable> CachedAdjectiveTable;

	EPBItemQuality GenerateItemQuality(EPBItemQuality MinQuality = EPBItemQuality::Quality0, EPBItemQuality MaxQuality = EPBItemQuality::Quality5);

	FGameplayTag ConvertQualityEnumToTag(EPBItemQuality InQuality);
};
