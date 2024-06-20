// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectBotSource/Inventory/PBItemDefinition.h"
#include "PBEquippableItem.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType)
class LYRAGAME_API UPBEquippableItem : public UPBItemDefinition
{
	GENERATED_BODY()

public:

	UPBEquippableItem()
	{
		ItemType = EPBItemType::UndefinedType;
	}

	UFUNCTION(BlueprintCallable)
	UPBEquipmentDefinition* GetEquipmentDefinition() const;

protected:
	/** Equipment to grant and equip */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	TObjectPtr<UPBEquipmentDefinition> EquipmentDefinition;
};
