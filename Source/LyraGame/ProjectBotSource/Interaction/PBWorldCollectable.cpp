// Copyright Epic Games, Inc. All Rights Reserved.

#include "PBWorldCollectable.h"
#include "IPBPickupable.h"

#include "Async/TaskGraphInterfaces.h"
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"
#include "ProjectBotSource/Inventory/PBItemInstance.h"
#include "ProjectBotSource/Modifiers/PBItemModInstance.h"

struct FInteractionQuery;

APBWorldCollectable::APBWorldCollectable()
{
}

FPBInventoryPickup APBWorldCollectable::GetPickupInventory() const
{
	return StaticInventory;
}

APBWorldCollectableInstance::APBWorldCollectableInstance()
{
	bReplicates = true;
}

void APBWorldCollectableInstance::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (IsUsingRegisteredSubObjectList() && ItemInstance)
	{
		for (UPBItemModInstance* Mod : ItemInstance->GetItemMods())
		{
			RemoveReplicatedSubObject(Mod);
		}
		RemoveReplicatedSubObject(ItemInstance);
	}
}

FPBInventoryPickup APBWorldCollectableInstance::GetPickupInventory() const
{
	if(ItemInstance)
	{
		return FPBInventoryPickup(ItemInstance);
	}

	return FPBInventoryPickup();
}

void APBWorldCollectableInstance::K2_OnItemInstanceSet_Implementation()
{
}

void APBWorldCollectableInstance::OnItemInstanceSet()
{

	if (IsUsingRegisteredSubObjectList() && ItemInstance)
	{
		AddReplicatedSubObject(ItemInstance);
		for (UPBItemModInstance* Mod : ItemInstance->GetItemMods())
		{
			AddReplicatedSubObject(Mod);
		}
	}

	K2_OnItemInstanceSet();
}

void APBWorldCollectableInstance::SetItemInstance(UPBInventoryItemInstance* InItem)
{
	ItemInstance = InItem;
	OnItemInstanceSet();
}

void APBWorldCollectableInstance::OnRep_ItemInstance()
{
	OnItemInstanceSet();
}

void APBWorldCollectableInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APBWorldCollectableInstance, ItemInstance);
}

void APBWorldCollectableInstance::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if(ItemInstance)
	{
		OnItemInstanceSet();
	}
}

bool APBWorldCollectableInstance::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch,
	FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	if (ItemInstance && IsValid(ItemInstance))
	{
		WroteSomething |= Channel->ReplicateSubobject(ItemInstance, *Bunch, *RepFlags);
		for (UPBItemModInstance* Mod : ItemInstance->GetItemMods())
		{
			WroteSomething |= Channel->ReplicateSubobject(Mod, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}
