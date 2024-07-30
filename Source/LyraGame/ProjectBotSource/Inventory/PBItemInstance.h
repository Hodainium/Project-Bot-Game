// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "System/GameplayTagStack.h"
#include "PBInventoryComponent.h"
#include "UObject/NoExportTypes.h"
#include "PBItemInstance.generated.h"

enum class EPBInventorySlotType : uint8;
enum class EPBItemQuality : uint8;
enum class EPBItemType : uint8;
class UPBItemModInstance;
class UPBItemDefinition;
class ULyraGameplayAbility;
class UPBEquipmentDefinition;
class UPBInventoryItemFragment;

DECLARE_DYNAMIC_DELEGATE_OneParam(FPBOnItemMeshLoaded, UStaticMesh*, StaticMeshHQ);

/**
 * 
 */
UCLASS(BlueprintType)
class LYRAGAME_API UPBInventoryItemInstance : public UObject
{
	GENERATED_BODY()

public:

	UPBInventoryItemInstance(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintAssignable)
	FOnTagStackChanged OnItemTagStackChanged;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	static UPBInventoryItemInstance* DuplicateItemInstance(UPBInventoryItemInstance* Instance, UObject* NewOuter);

	//~UObject interface
	virtual bool IsSupportedForNetworking() const override { return true; }
	//~End of UObject interface

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Adds a specified number of stacks to the tag (does nothing if StackCount is below 1)
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Inventory)
	void AddStatTagStack(FGameplayTag Tag, int32 StackCount);

	// Removes a specified number of stacks from the tag count is below 0 after it removes it
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Inventory)
	void RemoveStatTagStack(FGameplayTag Tag, int32 StackCount);

	// Returns the stack count of the specified tag (or 0 if the tag is not present)
	UFUNCTION(BlueprintCallable, Category = Inventory)
	int32 GetStatTagStackCount(FGameplayTag Tag) const;

	// Returns true if there is at least one stack of the specified tag
	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool HasStatTag(FGameplayTag Tag) const;

	UFUNCTION(BlueprintCallable, Category = Inventory)
	UPBItemDefinition* GetItemDefinition() const;

	UFUNCTION(BlueprintCallable, Category = Inventory)
	TArray<UPBItemModInstance*> GetItemMods() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Inventory)
	EPBItemType GetItemTypeEnum() const;

	UFUNCTION(BlueprintCallable, Category = Inventory)
	FText GetItemName() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Inventory)
	FText GetItemDescription() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Item|Inventory")
	void GetCompatibleSlotTypes(TArray<EPBInventorySlotType>& CompatibleTypes) const;

	UFUNCTION(BlueprintCallable, Category = Inventory)
	float GetItemWeight() const;

	UFUNCTION(BlueprintCallable, Category = "Item|Display|World")
	UStaticMesh* GetDisplayMesh() const;

	//Todo implement fsyncmix and have this callback to requesting item when hq loaded
	UFUNCTION(BlueprintCallable, Category = "Item|Display|World", meta = (AutoCreateRefTerm = "Delegate"))
	UStaticMesh* GetDisplayMeshHQ(const FPBOnItemMeshLoaded& Delegate) const;

	UFUNCTION(BlueprintCallable, Category = Inventory)
	int32 GetMaxStack() const;

	UFUNCTION(BlueprintCallable, Category = Inventory)
	EPBItemQuality GetItemQuality() const;

	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool GetCanBeStacked();

	UFUNCTION(BlueprintCallable, Category = Inventory)
	TSoftObjectPtr<UTexture2D> GetItemIcon();

	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool IsItemStackCompatible(UPBInventoryItemInstance* IncomingItem) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, meta = (DeterminesOutputType = FragmentClass))
	const UPBInventoryItemFragment* FindFragmentByClass(TSubclassOf<UPBInventoryItemFragment> FragmentClass) const;

	template <typename ResultClass>
	const ResultClass* FindFragmentByClass() const
	{
		return (ResultClass*)FindFragmentByClass(ResultClass::StaticClass());
	}

protected:

	UFUNCTION()
	void OnStatTagChange(FGameplayTag Tag, int32 Count);

private:
	void SetItemDef(UPBItemDefinition* InDef);

	void SetItemQuality(EPBItemQuality InQuality);

	void SetItemAdjectiveText(FName NameKey);

	void AddItemMod(UPBItemModInstance* InMod);

	UFUNCTION()
	void OnRep_ReplicatedAdjectiveIndexKey();

	friend struct FPBInventoryList;
	friend class UPBLootGenGameInstanceSubsystem;

protected:
	UPROPERTY(Replicated)
	uint8 ItemStackCount;

	UPROPERTY(Replicated)
	TArray<TObjectPtr<UPBItemModInstance>> ItemMods;

	UPROPERTY(ReplicatedUsing="OnRep_ReplicatedAdjectiveIndexKey")
	FName ReplicatedAdjectiveIndexKey;

	UPROPERTY()
	FText CachedAdjective;

	UPROPERTY(Replicated)
	EPBItemQuality ItemQuality;

	UPROPERTY(Replicated)
	FGameplayTagStackContainer StatTags;

	UPROPERTY(Replicated)
	TObjectPtr<UPBItemDefinition> ItemDefinition;
};

