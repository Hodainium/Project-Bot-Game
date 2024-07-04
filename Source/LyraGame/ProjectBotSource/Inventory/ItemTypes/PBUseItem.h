// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectBotSource/Inventory/ItemTypes/PBEquippableItem.h"
#include "PBUseItem.generated.h"

/**
 * 
 */
UCLASS()
class LYRAGAME_API UPBUseItem : public UPBEquippableItem
{
	GENERATED_BODY()

public:

	UPBUseItem()
	{
		ItemType = EPBItemType::UseItem;
	}

	virtual FText GetDisplayItemTypeText() const override;

	virtual void GetCompatibleSlotTypes(TArray<EPBInventorySlotType>& CompatibleTypes) const override;
};
