// Copyright Epic Games, Inc. All Rights Reserved.

#include "PBWorldCollectable.h"
#include "IPBPickupable.h"

#include "Async/TaskGraphInterfaces.h"
#include "Net/UnrealNetwork.h"

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

FPBInventoryPickup APBWorldCollectableInstance::GetPickupInventory() const
{
	if(ItemInstance)
	{
		return FPBInventoryPickup(ItemInstance);
	}

	return FPBInventoryPickup();
}

void APBWorldCollectableInstance::OnItemInstanceSet_Implementation()
{
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
