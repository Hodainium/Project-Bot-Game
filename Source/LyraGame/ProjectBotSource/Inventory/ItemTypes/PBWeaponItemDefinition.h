// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PBEquippableItem.h"
#include "ProjectBotSource/Inventory/PBItemDefinition.h"
#include "PBWeaponItemDefinition.generated.h"

class UPBEquipmentDefinition;
/**
 * 
 */
UCLASS()
class LYRAGAME_API UPBWeaponItemDefinition : public UPBEquippableItem
{
	GENERATED_BODY()

public:
	/** Constructor */
	UPBWeaponItemDefinition()
	{
		bUsesAmmo = false;
		ItemType = EPBItemType::Weapon;
		Damage = 0.f;
		CanBeStacked = false;
		MaxStack = 1;
	}

	/** Does this weapon use ammo? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bUsesAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	float Damage;


};
