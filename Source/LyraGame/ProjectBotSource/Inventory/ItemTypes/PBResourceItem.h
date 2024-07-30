// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectBotSource/Inventory/ItemTypes/PBEquippableItem.h"
#include "PBResourceItem.generated.h"

/**
 * 
 */
UCLASS()
class LYRAGAME_API UPBResourceItem : public UPBItemDefinition
{
	GENERATED_BODY()

public:

	UPBResourceItem()
	{
		ItemType = EPBItemType::Resource;
	}

	virtual FText GetDisplayItemTypeText() const override;
};
