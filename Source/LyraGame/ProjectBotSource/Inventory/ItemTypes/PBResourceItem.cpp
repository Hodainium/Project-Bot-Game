// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectBotSource/Inventory/ItemTypes/PBResourceItem.h"

#include "ProjectBotSource/Inventory/PBItemSlotComponent.h"


FText UPBResourceItem::GetDisplayItemTypeText() const
{
	return NSLOCTEXT("ItemTypeDisplay", "ResourceItemType", "Resource");
}
