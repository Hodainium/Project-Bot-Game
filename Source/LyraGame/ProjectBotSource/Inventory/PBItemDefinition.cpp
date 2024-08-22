// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectBotSource/Inventory/PBItemDefinition.h"
#include "ProjectBotSource/Logs/PBLogChannels.h"
#include "ProjectBotSource/System/PBAssetManager.h"

FGameplayTagContainer UPBItemDefinition::GetItemTags()
{
	return ItemTags;
}

FString UPBItemDefinition::GetIdentifierString() const
{
	return GetPrimaryAssetId().ToString();
}

const FPrimaryAssetType* UPBItemDefinition::GetEnumAssetType(EPBItemType Enum)
{
	switch(Enum)
	{
		case EPBItemType::Weapon:
		{
			return &UPBAssetManager::WeaponItemType;
		}
		case EPBItemType::Armor:
		{
			return &UPBAssetManager::ArmorItemType;
		}
		case EPBItemType::Resource:
		{
			return &UPBAssetManager::ResourceItemType;
		}
		case EPBItemType::ItemMod:
		{
			return &UPBAssetManager::ItemModItemType;
		}
		case EPBItemType::CharacterMod:
		{
			return &UPBAssetManager::CharacterModItemType;
		}
		case EPBItemType::Consumable:
		{
			return &UPBAssetManager::ConsumableItemType;
		}
		case EPBItemType::UseItem:
		{
			return &UPBAssetManager::UseItemType;
		}
		default:
		case EPBItemType::UndefinedType:
		{
			ensureMsgf(false, TEXT("ItemAsset DOES NOT HAVE ENUM TO ASSETTYPE DEFINED!!!!"));
			UE_LOG(LogPBGame, Error, TEXT("ItemAsset DOES NOT HAVE ENUM TO ASSETTYPE DEFINED!!!!"));
			return &UPBAssetManager::UndefinedItemType;
		}
	}
}

FPrimaryAssetId UPBItemDefinition::GetPrimaryAssetId() const
{
	// This is a DataAsset and not a blueprint so we can just use the raw FName
	// For blueprints you need to handle stripping the _C suffix

	return FPrimaryAssetId(*GetEnumAssetType(ItemType), GetFName());
}

const UPBInventoryItemFragment* UPBItemDefinition::FindFragmentByClass(
	TSubclassOf<UPBInventoryItemFragment> FragmentClass) const
{
	if (FragmentClass != nullptr)
	{
		for (UPBInventoryItemFragment* Fragment : Fragments)
		{
			if (Fragment && Fragment->IsA(FragmentClass))
			{
				return Fragment;
			}
		}
	}

	return nullptr;
}

FText UPBItemDefinition::GetDisplayItemTypeText() const
{
	return NSLOCTEXT("ItemTypeDisplay", "UndefinedType", "UNDEFINED");
}

void UPBItemDefinition::GetCompatibleSlotTypes(TArray<EPBInventorySlotType>& CompatibleTypes) const
{
}

bool UPBItemDefinition::IsConsumable() const
{
	if (MaxStack <= 0)
	{
		return true;
	}
	return false;
}

const UPBInventoryItemFragment* UPBInventoryFunctionLibrary::FindItemDefinitionFragment(
	UPBItemDefinition* ItemDef, TSubclassOf<UPBInventoryItemFragment> FragmentClass)
{
	if ((ItemDef != nullptr) && (FragmentClass != nullptr))
	{
		return ItemDef->FindFragmentByClass(FragmentClass);
	}
	return nullptr;
}
