// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectBotSource/Inventory/ItemTypes/PBConsumableItem.h"

#include "ProjectBotSource/Inventory/PBItemSlotComponent.h"


FText UPBConsumableItem::GetDisplayItemTypeText() const
{
	return NSLOCTEXT("ItemTypeDisplay", "ConsumableItemType", "Contraption");
}

void UPBConsumableItem::GetCompatibleSlotTypes(TArray<EPBInventorySlotType>& CompatibleTypes) const
{
	CompatibleTypes.Add(EPBInventorySlotType::Consumable);
}
