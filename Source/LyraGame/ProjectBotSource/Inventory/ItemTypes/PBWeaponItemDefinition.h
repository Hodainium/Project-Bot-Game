// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PBEquippableItem.h"
#include "ProjectBotSource/Inventory/PBItemDefinition.h"
#include "PBWeaponItemDefinition.generated.h"

class UPBEquipmentDefinition;




UENUM(BlueprintType)
enum class EPBWeaponHandedType : uint8
{
	LeftHanded,
	RightHanded,
	BothHanded,
	UndefHanded
};

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
		WeaponHandedType = EPBWeaponHandedType::UndefHanded;
	}

	/** Does this weapon use ammo? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bUsesAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	EPBWeaponHandedType WeaponHandedType;

	virtual FText GetDisplayItemTypeText() const override;

	virtual void GetCompatibleSlotTypes(TArray<EPBInventorySlotType>& CompatibleTypes) const override;
};
