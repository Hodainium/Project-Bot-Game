// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectBotSource/System/PBAssetManager.h"
#include "AbilitySystemGlobals.h"
#include "Logging/StructuredLog.h"
#include "ProjectBotSource/Logs/PBLogChannels.h"
#include "ProjectBotSource/Modifiers/PBItemModDefinition.h"
#include "ProjectBotSource/LootSubsystem/PBItemSearchQuery.h"

const FPrimaryAssetType	UPBAssetManager::UndefinedItemType = TEXT("UNDEFINED_Type");
const FPrimaryAssetType	UPBAssetManager::ResourceItemType = TEXT("Resource_Type");
const FPrimaryAssetType	UPBAssetManager::ArmorItemType = TEXT("Armor_Type");
const FPrimaryAssetType	UPBAssetManager::WeaponItemType = TEXT("Weapon_Type");
const FPrimaryAssetType	UPBAssetManager::ItemModItemType = TEXT("ItemMod_Type");
const FPrimaryAssetType	UPBAssetManager::CharacterModItemType = TEXT("CharacterMod_Type");
const FPrimaryAssetType	UPBAssetManager::ConsumableItemType = TEXT("ConsumableItem_Type");


UPBAssetManager::UPBAssetManager()
{
	DefaultPawnData = nullptr;
}

UPBAssetManager& UPBAssetManager::Get()
{
	check(GEngine);

	if (UPBAssetManager* Singleton = Cast<UPBAssetManager>(GEngine->AssetManager))
	{
		return *Singleton;
	}

	UE_LOG(LogPBGame, Fatal, TEXT("Invalid AssetManagerClassName in DefaultEngine.ini.  It must be set to PBAssetManager!"));

	// Fatal error above prevents this from being called.
	return *NewObject<UPBAssetManager>();
}

void UPBAssetManager::GetAllItemModsMatching(FPBItemSearchQuery& SearchQuery, TArray<FAssetData>& OutAssets)
{
	TArray<FAssetData> ModAssetData;
	GetPrimaryAssetDataList(FPrimaryAssetType(ItemModItemType), ModAssetData);
	for (const auto& Asset : ModAssetData)
	{
		////Item quality
		//EPBItemQuality OutQuality;
		//Asset.GetTagValue("Quality", OutQuality);

		bool matchesQuery = true;

		//Asset tags
		FString OutAssetTagsString;
		Asset.GetTagValue(GET_MEMBER_NAME_CHECKED(UPBItemModDefinition, AssetTags), OutAssetTagsString);
		FGameplayTagContainer AssetTags;
		AssetTags.FromExportString(OutAssetTagsString);

		//Tags to grant during query
		FString OutTagsToGrantDuringQueryString;
		Asset.GetTagValue(GET_MEMBER_NAME_CHECKED(UPBItemModDefinition, TagsToGrantDuringQuery), OutTagsToGrantDuringQueryString);
		FGameplayTagContainer TagsToGrantDuringQuery;
		TagsToGrantDuringQuery.FromExportString(OutAssetTagsString);

		//Required tags
		FString OutRequiredTagsString;
		Asset.GetTagValue(GET_MEMBER_NAME_CHECKED(UPBItemModDefinition, RequiredTags), OutRequiredTagsString);
		FGameplayTagContainer RequiredTags;
		RequiredTags.FromExportString(OutRequiredTagsString);

		//Blocked tags
		FString OutBlockedTagsString;
		Asset.GetTagValue(GET_MEMBER_NAME_CHECKED(UPBItemModDefinition, BlockedTags), OutBlockedTagsString);
		FGameplayTagContainer BlockedTags;
		BlockedTags.FromExportString(OutBlockedTagsString);


		if(!SearchQuery.AccumulatedTags.HasAll(RequiredTags))
		{
			matchesQuery = false;
		}


		if(matchesQuery)
		{
			OutAssets.Add(Asset);
		}

		/*if (AllowedTags.HasTag(WeaponTag))
		{
			OutAssets.Add(Asset);
		}*/

		
	}
}

void UPBAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	UAbilitySystemGlobals::Get().InitGlobalData();
}
