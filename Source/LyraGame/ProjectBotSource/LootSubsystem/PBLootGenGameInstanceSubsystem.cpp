// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectBotSource/LootSubsystem/PBLootGenGameInstanceSubsystem.h"
#include "ProjectBotSource/Tags/PB_Tags.h"
#include "ProjectBotSource/LootSubsystem/PBLootSettings.h"
#include "ProjectBotSource/Inventory/PBItemDefinition.h"
#include "ProjectBotSource/Inventory/PBItemInstance.h"
#include "Logging/StructuredLog.h"
#include "ProjectBotSource/LootSubsystem/PBItemSearchQuery.h"
#include "ProjectBotSource/Modifiers/PBItemModDefinition.h"
#include "ProjectBotSource/Modifiers/PBItemModInstance.h"
#include "ProjectBotSource/System/PBAssetManager.h"

DEFINE_LOG_CATEGORY(LogPBLootSubsystem);

void UPBLootGenGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const UPBLootSettings* LootSettings = GetDefault<UPBLootSettings>(); // Access via CDO
	// Access defaults from DefaultGame.ini
	CachedAdjectiveTable = LootSettings->AdjectiveTable.LoadSynchronous();

	UE_LOGFMT(LogPBLootSubsystem, Warning, "Loot system initted");
}

void UPBLootGenGameInstanceSubsystem::Deinitialize()
{
	Super::Deinitialize();

	UE_LOGFMT(LogPBLootSubsystem, Warning, "Loot system Deinitted");
}

FText UPBLootGenGameInstanceSubsystem::GetAdjectiveForKey(FName InKey)
{
	if (CachedAdjectiveTable)
	{
		if(FPBTextRow* Row = CachedAdjectiveTable->FindRow<FPBTextRow>(InKey, ""))
		{
			return Row->Text;
		}
	}

	return FText::GetEmpty();
}

FName UPBLootGenGameInstanceSubsystem::GetRandomAdjectiveRowKey()
{
	if (CachedAdjectiveTable)
	{
		TArray<FName> AdjectiveRowNames = CachedAdjectiveTable->GetRowNames();

		if (AdjectiveRowNames.Num() > 0)
		{
			int randIndex = FMath::RandRange(0, AdjectiveRowNames.Num() - 1);

			return AdjectiveRowNames[randIndex];
		}
	}

	return NAME_None;
}

int UPBLootGenGameInstanceSubsystem::GenerateNumMods(EPBItemQuality InQuality)
{
	return FMath::RandRange((static_cast<int>(InQuality) + 1) / 2, static_cast<int>(InQuality) + 1);
}

UPBInventoryItemInstance* UPBLootGenGameInstanceSubsystem::GenerateItemInstance(UPBItemDefinition* ItemDef)
{
	if(!ItemDef)
	{
		return nullptr;
	}

	UPBInventoryItemInstance* Instance = NewObject<UPBInventoryItemInstance>(GetWorld());
	Instance->SetItemDef(ItemDef);
	for (UPBInventoryItemFragment* Fragment : ItemDef->Fragments)
	{
		if (Fragment != nullptr)
		{
			Fragment->OnInstanceCreated(Instance);
		}
	}

	EPBItemQuality ItemQuality = GenerateItemQuality();
	Instance->SetItemQuality(ItemQuality);

	FName RowKey = GetRandomAdjectiveRowKey();

	if(RowKey.IsValid())
	{
		Instance->SetItemAdjectiveText(RowKey);
	}

	return Instance;
}

void UPBLootGenGameInstanceSubsystem::GetCompatibleModAssetsForItemInstance(UPBInventoryItemInstance* ItemInstance,
                                                                          TArray<FAssetData>& OutDataArray)
{
	
}

void UPBLootGenGameInstanceSubsystem::GenerateItemInstanceFromSoftDel(TSoftObjectPtr<UPBItemDefinition> ItemDefRef, const FPBItemInstanceGenerated& Delegate)
{
	// Async load the indicator, and pool the results so that it's easy to use and reuse the widgets.
	if (!ItemDefRef.IsNull())
	{
		AsyncLoad(ItemDefRef, [this, ItemDefRef, Delegate]() 
		{
			if (UPBItemDefinition* ItemDef = ItemDefRef.Get())
			{
				UPBInventoryItemInstance* WeaponInstance = GenerateItemInstance(ItemDef);

				TArray<FAssetData> TotalModData;

				FPBItemSearchQuery Query = FPBItemSearchQuery();

				Query.AccumulatedTags.AppendTags(ItemDef->GetItemTags());
				
				//Query.BlockedModQualities = GetBlockedItemQualitiesForRange(EPBItemQuality::Quality0, ItemInstance->GetItemQuality());
				UPBAssetManager::Get().GetAllItemModsMatching(Query, TotalModData);

				TArray<FPrimaryAssetId> SelectedModIDs;
				TArray<EPBItemQuality> SelectedQualities;

				int numMods = GenerateNumMods(WeaponInstance->GetItemQuality()); // +1 TODO: Make this a cvar

				UE_LOGFMT(LogPBLootSubsystem, Warning, "Num mods: {0}", numMods);

				
				
				//Find mods to add
				for (int i = 0; i < numMods; i++)
				{
					bool bModFound = false;

					//static_cast<EPBItemQuality>((static_cast<int>(WeaponInstance->GetItemQuality()) + 1))

					EPBItemQuality MinQuality = static_cast<EPBItemQuality>(static_cast<int>(WeaponInstance->GetItemQuality()) / 2);

					EPBItemQuality RolledModQuality = GenerateItemQuality(MinQuality, WeaponInstance->GetItemQuality()); //GenRarity()

					int offsetFromEndIdx = 0;

					if(TotalModData.Num() > 0) //Make sure we found at least one mod. Prevents crash
					{
						do
						{
							int maxIndex = TotalModData.Num() - 1 - offsetFromEndIdx;
							int currentIndex = FMath::RandRange(0, maxIndex);

							//Item quality
							FString OutQualitiesString;
							TotalModData[currentIndex].GetTagValue(GET_MEMBER_NAME_CHECKED(UPBItemModDefinition, AvailableQualities), OutQualitiesString);
							FGameplayTagContainer QualityTags;
							QualityTags.FromExportString(OutQualitiesString);

							if (QualityTags.HasTag(ConvertQualityEnumToTag(RolledModQuality)))
							{
								SelectedModIDs.Add(TotalModData[currentIndex].GetPrimaryAssetId());
								SelectedQualities.Add(RolledModQuality);
								TotalModData.RemoveAt(currentIndex);
								bModFound = true;

								UE_LOGFMT(LogPBLootSubsystem, Warning, "Mod found!!!");
							}
							else
							{
								TotalModData.Swap(currentIndex, maxIndex);

								offsetFromEndIdx++;
							}
						} while (!bModFound && offsetFromEndIdx < TotalModData.Num());
					}
					
				}

				TArray<FName> Bundles;

				AsyncPreloadPrimaryAssetsAndBundles(SelectedModIDs, Bundles, [this, SelectedModIDs, SelectedQualities, WeaponInstance, Delegate]()
				{
					for (int i = 0; i < SelectedModIDs.Num(); i++)
					{
						if (UPBItemModDefinition* ModDef = Cast<UPBItemModDefinition>(UPBAssetManager::Get().GetPrimaryAssetObject(SelectedModIDs[i])))
						{
							UPBItemModInstance* ModInstance = NewObject<UPBItemModInstance>(WeaponInstance);  //@TODO: Using the actor instead of component as the outer due to UE-127172
							ModInstance->SetModDefinition(ModDef);
							ModInstance->SetModQuality(SelectedQualities[i]);

							//ModInstance->SetModLevelOffset();
							WeaponInstance->AddItemMod(ModInstance);
						}
					}
					Delegate.ExecuteIfBound(WeaponInstance);
				});
				StartAsyncLoading();
			}
		});
		StartAsyncLoading();
	}
}

EPBItemQuality UPBLootGenGameInstanceSubsystem::GenerateItemQuality(EPBItemQuality MinQuality, EPBItemQuality MaxQuality)
{
	//Can this be an int?
	float randNum = FMath::RandRange(0.f, 100.f);

	EPBItemQuality ItemQuality;

	//Maybe move to function. GetItemQualityForVal(randNum)?
	//This will need to be more extendable to allow for items like the isaac sacred orb that enhances item rolls
	//TODO add addition here for each weight. EX: randum < 1f + q4extraWeight

	if (randNum < 1.f) //+ extra q4 weight
	{
		ItemQuality = EPBItemQuality::Quality4;
	}
	else if (randNum < 7.77f) //+ extra q4 weight + extra q3 weight?
	{
		ItemQuality = EPBItemQuality::Quality3;
	}
	else if (randNum < 20.f)
	{
		ItemQuality = EPBItemQuality::Quality2;
	}
	else if (randNum < 50.f)
	{
		ItemQuality = EPBItemQuality::Quality1;
	}
	else
	{
		ItemQuality = EPBItemQuality::Quality0;
	}

	if (ItemQuality < MinQuality)
	{
		ItemQuality = MinQuality;
	}

	if(ItemQuality > MaxQuality)
	{
		ItemQuality = MaxQuality;
	}

	return ItemQuality;
}

FGameplayTag UPBLootGenGameInstanceSubsystem::ConvertQualityEnumToTag(EPBItemQuality InQuality)
{
	switch(InQuality)
	{
	case EPBItemQuality::Quality0:
		return PB_ItemQuality_Tags::TAG_ITEM_QUALITY_0;
	case EPBItemQuality::Quality1:
		return PB_ItemQuality_Tags::TAG_ITEM_QUALITY_1;
	case EPBItemQuality::Quality2:
		return PB_ItemQuality_Tags::TAG_ITEM_QUALITY_2;
	case EPBItemQuality::Quality3:
		return PB_ItemQuality_Tags::TAG_ITEM_QUALITY_3;
	case EPBItemQuality::Quality4:
		return PB_ItemQuality_Tags::TAG_ITEM_QUALITY_4;
	case EPBItemQuality::Quality5:
		return PB_ItemQuality_Tags::TAG_ITEM_QUALITY_5;
	default:
		{
			UE_LOGFMT(LogPBLootSubsystem, Error, "Invalid or not implemented loot enum passed to generate tag. Giving Q0");

			return PB_ItemQuality_Tags::TAG_ITEM_QUALITY_0;
		}
	}
}
