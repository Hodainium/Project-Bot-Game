// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectBotSource/Inventory/ItemTypes/PBUseItem.h"

#include "ProjectBotSource/Inventory/PBItemSlotComponent.h"


FText UPBUseItem::GetDisplayItemTypeText() const
{
	return NSLOCTEXT("ItemTypeDisplay", "UseItemType", "Gadget");
}

void UPBUseItem::GetCompatibleSlotTypes(TArray<EPBInventorySlotType>& CompatibleTypes) const
{
	CompatibleTypes.Add(EPBInventorySlotType::UseItem);
}
