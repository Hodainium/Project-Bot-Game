// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectBotSource/Inventory/ItemTypes/PBWeaponItemDefinition.h"

#include "ProjectBotSource/Inventory/PBItemSlotComponent.h"

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

void UPBWeaponItemDefinition::GetCompatibleSlotTypes(TArray<EPBInventorySlotType>& CompatibleTypes) const
{
	switch (WeaponHandedType)
	{
	case EPBWeaponHandedType::LeftHanded:
		CompatibleTypes.Add(EPBInventorySlotType::Weapon_L);
		break;
	case EPBWeaponHandedType::RightHanded:
		CompatibleTypes.Add(EPBInventorySlotType::Weapon_R);
		break;
	case EPBWeaponHandedType::BothHanded:
		{
		CompatibleTypes.Add(EPBInventorySlotType::Weapon_L);
		CompatibleTypes.Add(EPBInventorySlotType::Weapon_R);
		break;
		}
	case EPBWeaponHandedType::UndefHanded:
	default:
		break;
	}
}

UPBEquipmentDefinition* UPBWeaponItemDefinition::GetEquipmentDefinitionTwoHanded() const
{
	return TwoHandedEquipmentDefinition;
}
