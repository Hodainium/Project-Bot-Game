// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectBotSource/Inventory/PBItemInstance.h"
#include "ProjectBotSource/Inventory/PBInventoryComponent.h"
#include "ProjectBotSource/Logs/PBLogChannels.h"
#include "Logging/StructuredLog.h"
#include "ProjectBotSource/Modifiers/PBItemModDefinition.h"
#include "ProjectBotSource/LootSubsystem/PBLootGenGameInstanceSubsystem.h"
#include "ProjectBotSource/Inventory/PBItemDefinition.h"
#include "Net/UnrealNetwork.h"
#include "ProjectBotSource/Modifiers/PBItemModInstance.h"

UPBInventoryItemInstance::UPBInventoryItemInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), ItemQuality(EPBItemQuality::Quality0)
{
}

UPBInventoryItemInstance* UPBInventoryItemInstance::DuplicateItemInstance(UPBInventoryItemInstance* Instance, UObject* NewOuter)
{
	UPBInventoryItemInstance* Result = DuplicateObject(Instance, NewOuter);
	Result->ItemMods.Reset();
	for (UPBItemModInstance* OldMod : Instance->ItemMods)
	{
		Result->ItemMods.Add(DuplicateObject(OldMod, NewOuter));
	}

	return Result;
}

void UPBInventoryItemInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, StatTags);
	DOREPLIFETIME(ThisClass, ItemDefinition);
	DOREPLIFETIME(ThisClass, ItemQuality);
	DOREPLIFETIME(ThisClass, ItemMods);
	DOREPLIFETIME(ThisClass, ReplicatedAdjectiveIndexKey);
}

void UPBInventoryItemInstance::AddStatTagStack(FGameplayTag Tag, int32 StackCount)
{
	StatTags.AddStack(Tag, StackCount);
}

void UPBInventoryItemInstance::RemoveStatTagStack(FGameplayTag Tag, int32 StackCount)
{
	StatTags.RemoveStack(Tag, StackCount);
}

int32 UPBInventoryItemInstance::GetStatTagStackCount(FGameplayTag Tag) const
{
	return StatTags.GetStackCount(Tag);
}

bool UPBInventoryItemInstance::HasStatTag(FGameplayTag Tag) const
{
	return StatTags.ContainsTag(Tag);
}

UPBItemDefinition* UPBInventoryItemInstance::GetItemDefinition() const
{
	return ItemDefinition;
}

TArray<UPBItemModInstance*> UPBInventoryItemInstance::GetItemMods() const
{
	return ItemMods;
}

EPBItemType UPBInventoryItemInstance::GetItemTypeEnum() const
{
	if (ItemDefinition)
	{
		return ItemDefinition->ItemType;
	}

	return EPBItemType::UndefinedType;
}

FText UPBInventoryItemInstance::GetItemName() const
{
	if (ItemDefinition)
	{
		if(!CachedAdjective.IsEmpty())
		{
			return FText::Format(NSLOCTEXT("WeaponUI", "WeaponTitle", "{0} {1}"), CachedAdjective, ItemDefinition->ItemName);
		}

		return ItemDefinition->ItemName;
	}

	return FText::GetEmpty();
}

FText UPBInventoryItemInstance::GetItemDescription() const
{
	if(ItemDefinition)
	{
		return ItemDefinition->ItemDescription;
	}

	return FText::GetEmpty();
}

void UPBInventoryItemInstance::GetCompatibleSlotTypes(TArray<EPBInventorySlotType>& CompatibleTypes) const
{
	if(ItemDefinition)
	{
		ItemDefinition->GetCompatibleSlotTypes(CompatibleTypes);
	}
}

float UPBInventoryItemInstance::GetItemWeight() const
{
	if (ItemDefinition)
	{
		return ItemDefinition->Weight;
	}

	return 0.f;
}

UStaticMesh* UPBInventoryItemInstance::GetDisplayMesh() const
{
	if(ItemDefinition)
	{
		return ItemDefinition->LowQualityWorldModel;
	}

	return nullptr;
}

UStaticMesh* UPBInventoryItemInstance::GetDisplayMeshHQ(const FPBOnItemMeshLoaded& Delegate) const
{
	if (ItemDefinition)
	{
		if(UStaticMesh* HQMesh = ItemDefinition->HighQualityWorldModel.Get())
		{
			return HQMesh;
		}
		//todo start async load here and link callback
		return ItemDefinition->LowQualityWorldModel;
	}


	return nullptr;
}

int32 UPBInventoryItemInstance::GetMaxStack() const
{
	if(ItemDefinition)
	{
		return ItemDefinition->MaxStack;
	}

	return INDEX_NONE;
}

EPBItemQuality UPBInventoryItemInstance::GetItemQuality() const
{
	return ItemQuality;
}

bool UPBInventoryItemInstance::GetCanBeStacked()
{
	return ItemDefinition->CanBeStacked;
}

TSoftObjectPtr<UTexture2D> UPBInventoryItemInstance::GetItemIcon()
{
	if (ItemDefinition)
	{
		return ItemDefinition->ItemIcon;
	}

	return nullptr;
}

bool UPBInventoryItemInstance::IsItemStackCompatible(UPBInventoryItemInstance* IncomingItem) const
{
	if (UPBItemDefinition* IncomingItemDef = IncomingItem->GetItemDefinition())
	{
		if(ItemDefinition == IncomingItemDef)
		{
			UE_LOGFMT(LogPBGame, Warning, "INVENTORY: Can stack item");
			return true;
		}
	}

	UE_LOGFMT(LogPBGame, Warning, "INVENTORY: Cannot stack item");

	return false;
}

const UPBInventoryItemFragment* UPBInventoryItemInstance::FindFragmentByClass(
	TSubclassOf<UPBInventoryItemFragment> FragmentClass) const
{
	if ((ItemDefinition != nullptr) && (FragmentClass != nullptr))
	{
		return ItemDefinition->FindFragmentByClass(FragmentClass);
	}

	return nullptr;
}

void UPBInventoryItemInstance::SetItemDef(UPBItemDefinition* InDef)
{
	ItemDefinition = InDef;
}

void UPBInventoryItemInstance::SetItemQuality(EPBItemQuality InQuality)
{
	ItemQuality = InQuality;
}

void UPBInventoryItemInstance::SetItemAdjectiveText(FName NameKey)
{
	ReplicatedAdjectiveIndexKey = NameKey;
	OnRep_ReplicatedAdjectiveIndexKey();
}

void UPBInventoryItemInstance::AddItemMod(UPBItemModInstance* InMod)
{
	ItemMods.Add(InMod);
}

void UPBInventoryItemInstance::OnRep_ReplicatedAdjectiveIndexKey()
{
	if(UPBLootGenGameInstanceSubsystem* LootSystem = GetWorld()->GetGameInstance()->GetSubsystem<UPBLootGenGameInstanceSubsystem>())
	{
		CachedAdjective = LootSystem->GetAdjectiveForKey(ReplicatedAdjectiveIndexKey);
		//TODO: Broadcast text changed maybe
	}
}

