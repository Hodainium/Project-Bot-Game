// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectBotSource/Inventory/ItemTypes/PBEquippableItem.h"
#include "PBConsumableItem.generated.h"

/**
 * 
 */
UCLASS()
class LYRAGAME_API UPBConsumableItem : public UPBEquippableItem
{
	GENERATED_BODY()

	UPBConsumableItem()
	{
		ItemType = EPBItemType::Consumable;
	}
};
