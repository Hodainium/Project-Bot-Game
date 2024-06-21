// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectBotSource/Inventory/ItemTypes/PBWeaponItemDefinition.h"

FText UPBWeaponItemDefinition::GetDisplayItemTypeText() const
{
	switch(WeaponHandedType)
	{
	case EPBWeaponHandedType::LeftHanded:
		return NSLOCTEXT("ItemTypeDisplay", "LeftWeaponType", "Left Handed Weapon");
	case EPBWeaponHandedType::RightHanded:
		return NSLOCTEXT("ItemTypeDisplay", "RightWeaponType", "Right Handed Weapon");
	case EPBWeaponHandedType::BothHanded:
		return NSLOCTEXT("ItemTypeDisplay", "RightWeaponType", "Either Handed Weapon");
	case EPBWeaponHandedType::UndefHanded:
	default:
		return Super::GetDisplayItemTypeText();
	}
}
