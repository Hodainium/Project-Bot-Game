// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "PBInventoryEnums.h"
#include "UObject/NoExportTypes.h"
#include "PBItemDefinition.generated.h"


enum class EPBItemQuality : uint8;
class UPBInventoryItemInstance;
class UPBEquipmentDefinition;
/**
 * 
 */

//////////////////////////////////////////////////////////////////////

//IF YOU CHANGE ANY OF THESE NEED TO CHANGE GETENUMASSETTYPE FUNC IN THIS CLASS
//ALSO NEED TO ADD/REMOVE STATIC ASSETTYPES IN HASSETMANAGER
//NEED TO OVERRIDE ENUM IN NEW ASSET TYPE

//Enum item type that is mapped to PrimaryAssetType
UENUM(BlueprintType)
enum class EPBItemType : uint8
{
	Weapon,
	Armor,
	Resource,
	ItemMod,
	CharacterMod,
	UndefinedType
};

//////////////////////////////////////////////////////////////////////

// Represents a fragment of an item definition
UCLASS(DefaultToInstanced, EditInlineNew, Abstract)
class LYRAGAME_API UPBInventoryItemFragment : public UObject
{
	GENERATED_BODY()

public:
	virtual void OnInstanceCreated(UPBInventoryItemInstance* Instance) const {}
};

/**
 * UPBInventoryItemDefinition
 */

UCLASS(Abstract, BlueprintType)
class LYRAGAME_API UPBItemDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPBItemDefinition()
		: BaseItemQuality(EPBItemQuality::Quality0), Price(0)
		  , CanBeStacked(false), MaxStack(1)
		  , MaxDurability(0)
		  , Weight(0), MaxLevel(1)
		  , AbilityLevel(1)
	{
		ItemType = EPBItemType::UndefinedType;
	}

	/** Type of this item, set in native parent class */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item, AssetRegistrySearchable)
	EPBItemType ItemType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Item)
	FGameplayTagContainer FlavorTags;

	/** User-visible short name */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	FText ItemName;

	/** User-visible long description */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	FText ItemDescription;

	/** Icon to display */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	TSoftObjectPtr<UTexture2D> ItemIcon;

	/** Pickup mesh to display */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Graphics|World")
	TObjectPtr<UStaticMesh> LowQualityWorldModel;

	/** Pickup mesh to display */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item|Graphics|World")
	TSoftObjectPtr<UStaticMesh> HighQualityWorldModel;

	/** Item Quality */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	EPBItemQuality BaseItemQuality;

	/** Price in game */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	int32 Price;

	/** Can this item stack? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	bool CanBeStacked;

	/** Maximum number of instances that can be in inventory at once, <= 0 means infinite */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Max)
	int32 MaxStack;

	/** Maximum number of instances that can be in inventory at once, <= 0 means infinite */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Durability)
	float MaxDurability;

	////Sound to play when picked up
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup")
	//TObjectPtr<USoundBase> PickedUpSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	float Weight;

	/** Returns if the item is consumable (MaxStack <= 0)*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = Max)
	bool IsConsumable() const;

	/** Maximum level this item can be, <= 0 means infinite */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Max)
	int32 MaxLevel;

	/** Ability level this item grants. <= 0 means the character level */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Abilities)
	int32 AbilityLevel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item|Fragments")
	TArray<TObjectPtr<UPBInventoryItemFragment>> Fragments;

	/** Returns the logical name, equivalent to the primary asset id */
	UFUNCTION(BlueprintCallable, Category = Item)
	FString GetIdentifierString() const;

	/** Overridden to use saved type */
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	const UPBInventoryItemFragment* FindFragmentByClass(TSubclassOf<UPBInventoryItemFragment> FragmentClass) const;

private:
	static const FPrimaryAssetType* GetEnumAssetType(EPBItemType Enum);
};

UCLASS()
class UPBInventoryFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, meta = (DeterminesOutputType = FragmentClass))
	static const UPBInventoryItemFragment* FindItemDefinitionFragment(UPBItemDefinition* ItemDef, TSubclassOf<UPBInventoryItemFragment> FragmentClass);
};
